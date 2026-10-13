#include <cmath>

#include "engine/memory/arena.h"
#include "engine/core/common.h"
#include "game/app/gameState.h"

#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <system_error>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_ttf/SDL_ttf.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using LibraryHandle = HMODULE;
constexpr const char* LIB_PREFIX = "";
constexpr const char* LIB_EXTENSION = ".dll";
#else
#include <dlfcn.h>
using LibraryHandle = void*;
constexpr const char* LIB_PREFIX = "lib";
constexpr const char* LIB_EXTENSION = ".so";
#endif

namespace fs = std::filesystem;

SDL_Window* window;
SDL_Renderer* renderer;
Uint64 NOW;
Uint64 PREV;
static int load_counter = 0;

typedef void ( *Function_Initialize )( GameData* data, SDL_Window* window, SDL_Renderer* renderer );

typedef bool ( *Function_HandleEvents )( Arena* arena, SDL_Event& event );

typedef void ( *Function_Update )( GameData* data, float dt );

typedef void ( *Function_Draw )( GameData* data, SDL_Renderer* renderer );

typedef void ( *Function_OnQuit )( SDL_Renderer* renderer );

struct DLL_INFO {
    LibraryHandle handle = nullptr;
    long long Timestamp = 0;
    Function_Initialize Initialize = nullptr;
    Function_HandleEvents HandleEvents = nullptr;
    Function_Update Update = nullptr;
    Function_Draw Draw = nullptr;
    Function_OnQuit OnQuit = nullptr;
};

std::string GameLibraryName() {
    return std::string("./") + LIB_PREFIX + "FantasyArena_game" + LIB_EXTENSION;
}

LibraryHandle OpenLibrary( const char* path ) {
#ifdef _WIN32
    return LoadLibraryA(path);
#else
    return dlopen(path, RTLD_NOW);
#endif
}

void* GetSymbol( LibraryHandle handle, const char* name ) {
#ifdef _WIN32
    return reinterpret_cast<void*>(GetProcAddress(handle, name));
#else
    return dlsym(handle, name);
#endif
}

void CloseLibrary( LibraryHandle handle ) {
#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

long long GetTimestamp() {
    std::error_code error;
    fs::path path = GameLibraryName();
    if (!fs::exists(path, error)) {
        return 0;
    }
    fs::file_time_type write_time = fs::last_write_time(path, error);
    if (error) {
        return 0;
    }
    return std::chrono::duration_cast<std::chrono::milliseconds>(write_time.time_since_epoch()).count();
}

bool CopyFile( const std::string& from, const std::string& to ) {
    std::error_code error;
    fs::copy_file(from, to, fs::copy_options::overwrite_existing, error);
    return !error;
}

bool TryLoadDLL( DLL_INFO* info ) {
    char temp_name[256];
    snprintf(temp_name, sizeof(temp_name), "./%sFantasyArena_game_temp_%d%s", LIB_PREFIX, load_counter++,
             LIB_EXTENSION);

    if (!CopyFile(GameLibraryName(), temp_name)) {
        return false;
    }

    LibraryHandle handle = OpenLibrary(temp_name);
    if (handle == nullptr) {
        return false;
    }

    Function_Initialize initialize = (Function_Initialize) GetSymbol(handle, "Initialize");
    Function_HandleEvents handleEvents = (Function_HandleEvents) GetSymbol(handle, "HandleEvents");
    Function_Update update = (Function_Update) GetSymbol(handle, "Update");
    Function_Draw draw = (Function_Draw) GetSymbol(handle, "Draw");
    Function_OnQuit onQuit = (Function_OnQuit) GetSymbol(handle, "OnQuit");

    if (initialize == nullptr || handleEvents == nullptr || update == nullptr || draw == nullptr ||
        onQuit == nullptr) {
        SDL_Log("game library is missing entry points: %s", temp_name);
        CloseLibrary(handle);
        return false;
    }

    info->handle = handle;
    info->Initialize = initialize;
    info->HandleEvents = handleEvents;
    info->Update = update;
    info->Draw = draw;
    info->OnQuit = onQuit;
    info->Timestamp = GetTimestamp();
    return true;
}

bool LoadDLL( DLL_INFO* info ) {
    for (int attempt = 0; attempt < 40; attempt++) {
        DLL_INFO candidate;
        if (TryLoadDLL(&candidate)) {
            // Vi stänger medvetet INTE den gamla libben: spelläget pekar på
            // strängliteraler i den (t.ex. knapptexter), så att avlasta den
            // skulle göra de pekarna ogiltiga. Den läcker därför per reload.
            *info = candidate;
            return true;
        }
        SDL_Delay(25);
    }
    return false;
}

void DLL_CheckStatus( DLL_INFO* dll ) {
    long long timestamp = GetTimestamp();
    if (dll->handle != nullptr && dll->Timestamp != timestamp) {
        SDL_Log("reloading game library");
        if (LoadDLL(dll)) {
            SDL_Log("reloaded game library");
        } else {
            SDL_Log("reload failed, keeping previous build and retrying");
        }
    }
}

void* AllocateGameMemory( size_t size ) {
    void* blob = malloc(size);
    if (blob == nullptr) {
        SDL_Log("fatal error: could not allocate memory");
        return nullptr;
    }
    return blob;
}

void SDL_Setup() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("pilot", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, nullptr);
}

void CalculateDeltaTime( float* dt, float scaler ) {
    NOW = SDL_GetTicksNS();
    *dt = NOW - PREV;
    *dt = SDL_NS_TO_SECONDS(*dt);
    *dt *= scaler;
    PREV = NOW;
}

void CalculateRemainingFrameTime_MS( double* milliseconds ) {
    Uint64 frame_end_time_ns = SDL_GetTicksNS();
    double frame_time_spent_ns = frame_end_time_ns - PREV;
    double frame_time_spent_ms = frame_time_spent_ns / 1e6;
    *milliseconds = FRAME_TIME_MS - frame_time_spent_ms;
}

int main() {
    void* game_memory = AllocateGameMemory(GAME_MEMORY_ALLOWANCE);
    if (!game_memory)
        return 1;

    // Main memory
    Arena* arena_main = new Arena();
    Initialize(arena_main, game_memory, GAME_MEMORY_ALLOWANCE);
    GameData* gameData = ALLOC(arena_main, GameData);
    gameData->arena_main = arena_main;
    gameData->ticks_total = ALLOC(arena_main, uint64_t);
    gameData->arena_scratch = CreateSubArena(arena_main, KILOBYTES(256));

    Gameplay* gameplay = &gameData->scenes.gameplay;

    //Sprites memory
    size_t IMAGE_ARENA_SIZE = MEGABYTES(1);
    gameData->arena_images = CreateSubArena(arena_main, IMAGE_ARENA_SIZE);
    gameData->tilesetBuffer = ALLOC_ARRAY(gameData->arena_images, Tileset, (int) TILESETS::COUNT);

    //Levels memory
    gameData->arena_levels = CreateSubArena(arena_main, MEGABYTES(3));
    gameData->arena_entities = CreateSubArena(gameData->arena_levels, MEGABYTES(1));
    gameplay->levels = ALLOC_ARRAY(gameData->arena_levels, LevelData, MAX_LEVELS);

    //Input management memory
    size_t INPUT_ARENA_SIZE = 0;
    INPUT_ARENA_SIZE += sizeof(bool) * SDL_SCANCODE_COUNT * 2;
    INPUT_ARENA_SIZE += sizeof(float) * SDL_SCANCODE_COUNT;
    INPUT_ARENA_SIZE += 128;
    gameData->arena_input = CreateSubArena(arena_main, INPUT_ARENA_SIZE);
    gameData->input.keys_current = ALLOC_ARRAY(gameData->arena_input, bool, SDL_SCANCODE_COUNT);
    gameData->input.keys_previous = ALLOC_ARRAY(gameData->arena_input, bool, SDL_SCANCODE_COUNT);
    gameData->input.keys_held_time = ALLOC_ARRAY(gameData->arena_input, float, SDL_SCANCODE_COUNT);
    gameData->input.mouse_held_time = ALLOC_ARRAY(gameData->arena_input, float, (int) MouseButton::COUNT);

    DLL_INFO dll;

    bool dllLoaded = LoadDLL(&dll);
    if (!dllLoaded) {
        return 2;
    }

    SDL_Setup();
    TTF_Init();

    dll.Initialize(gameData, window, renderer);

    gameData->running = true;
    float dt;
    float dt_scaler = 1;
    gameData->dt = &dt;
    gameData->dt_scaler = &dt_scaler;

    while (gameData->running) {
        CalculateDeltaTime(&dt, dt_scaler);
        DLL_CheckStatus(&dll);
        Reset(gameData->arena_scratch);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            gameData->running = dll.HandleEvents(arena_main, event);
            if (gameData->running == false) {
                break;
            }
        }

        gameData->input.keys_current = SDL_GetKeyboardState(nullptr);

        float* delta_x = &gameData->input.mouse_x_delta;
        float* delta_y = &gameData->input.mouse_y_delta;
        *delta_x = gameData->input.mouse_x;
        *delta_y = gameData->input.mouse_y;
        gameData->input.mouse_current = SDL_GetMouseState(&gameData->input.mouse_x, &gameData->input.mouse_y);
        *delta_x = gameData->input.mouse_x - *delta_x;
        *delta_y = gameData->input.mouse_y - *delta_y;
        float dx = *delta_x;
        float dy = *delta_y;
        gameData->input.mouse_magnitude = std::sqrt(dx * dx + dy * dy);

        dll.Update(gameData, dt);
        UpdateKeys(&gameData->input, dt);
        UpdateMouse(&gameData->input, dt);
        dll.Draw(gameData, renderer);

        double time_to_sleep_ms;
        CalculateRemainingFrameTime_MS(&time_to_sleep_ms);
        if (time_to_sleep_ms > 0) {
            if (time_to_sleep_ms > 1) {
                SDL_Delay(time_to_sleep_ms - 1);
            }
            while (time_to_sleep_ms > 0) {
                CalculateRemainingFrameTime_MS(&time_to_sleep_ms);
            }
        } else {
            printf("missed frame \n");
        }
    }

    dll.OnQuit(renderer);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
