#include <cmath>

#include "engine/memory/arena.h"
#include "engine/core/common.h"
#include "game/app/gameState.h"

#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
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
    LibraryHandle handle;
    std::time_t Timestamp;
    Function_Initialize Initialize;
    Function_HandleEvents HandleEvents;
    Function_Update Update;
    Function_Draw Draw;
    Function_OnQuit OnQuit;
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

std::time_t GetTimestamp() {
    std::error_code error;
    fs::path path = GameLibraryName();
    if (!fs::exists(path, error)) {
        return 0;
    }
    fs::file_time_type write_time = fs::last_write_time(path, error);
    if (error) {
        return 0;
    }
    return std::chrono::duration_cast<std::chrono::seconds>(write_time.time_since_epoch()).count();
}

bool CopyFile( const std::string& from, const std::string& to ) {
    std::error_code error;
    fs::copy_file(from, to, fs::copy_options::overwrite_existing, error);
    return !error;
}

bool LoadDLL( DLL_INFO* info, int depth = 0 ) {
    if (depth > 20) {
        SDL_Log("failed to write temp library.");
        return false;
    }

    // Unikt temp-namn varje gång så vi aldrig skriver över en laddad fil
    char temp_name[256];
    snprintf(temp_name, sizeof(temp_name), "./%sFantasyArena_game_temp_%d%s", LIB_PREFIX, load_counter++,
             LIB_EXTENSION);

    bool success = CopyFile(GameLibraryName(), temp_name);
    if (!success) {
        SDL_Delay(50);
        return LoadDLL(info, depth + 1);
    }

    info->handle = OpenLibrary(temp_name);

    if (!info->handle) {
        SDL_Log("could not load library: %s", temp_name);
        return false;
    }

    info->Initialize = (Function_Initialize) GetSymbol(info->handle, "Initialize");
    info->HandleEvents = (Function_HandleEvents) GetSymbol(info->handle, "HandleEvents");
    info->Update = (Function_Update) GetSymbol(info->handle, "Update");
    info->Draw = (Function_Draw) GetSymbol(info->handle, "Draw");
    info->OnQuit = (Function_OnQuit) GetSymbol(info->handle, "OnQuit");
    info->Timestamp = GetTimestamp();
    return true;
}

void UnloadDLL( DLL_INFO* info ) {
    CloseLibrary(info->handle);
    info->handle = nullptr;
}

void DLL_CheckStatus( DLL_INFO* dll ) {
    std::time_t timestamp = GetTimestamp();
    bool is_timestamp_changed = dll->Timestamp != timestamp;
    if (is_timestamp_changed) {
        SDL_Delay(100);
        UnloadDLL(dll);
        LoadDLL(dll);
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

    //Commands memory
    gameData->arena_commands = CreateSubArena(gameData->arena_levels, MEGABYTES(1));
    gameplay->commandBuffer = ALLOC(gameData->arena_commands, CommandBuffer);
    gameplay->commandBuffer->capacity = 2000;
    gameplay->commandBuffer->allCommands = ALLOC_ARRAY(gameData->arena_commands, AnyCommand,
                                                       gameplay->commandBuffer->capacity);

    //Input buffer memory
    gameplay->input_buffer_capacity = 50;
    gameplay->input_buffer = ALLOC_ARRAY(gameData->arena_levels, Position, gameplay->input_buffer_capacity);

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
