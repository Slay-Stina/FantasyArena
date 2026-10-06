#pragma once

#include "engine/graphics/spriteLibrary.h"
#include "engine/input/input.h"
#include "engine/graphics/camera.h"
#include <imgui.h>

#include "engine/audio/audioSystem.h"
#include "engine/graphics/FontAtlas.h"
#include "game/gameplay/game.h"
#include "game/editor/levelEditor.h"
#include "game/menu/mainmenu.h"
#include "game/app/credits.h"
#include "game/characterCreator/charCreator.h"

struct TitleScreen {
};

constexpr int FPS_BUFFER_COUNT = 100;

struct EditorData {
    Editor editor;
    bool edit_level;
    bool show_dev = true;
    float fps_buffer[FPS_BUFFER_COUNT] = {};
    int fps_buffer_index = 0;
};

struct Transition {
    enum States {
        Inactive,
        FadeTo,
        FadeFrom
    };

    States state = Inactive;
    float fade_time_elapsed = 0;
    float fade_time_duration = 1;
};

struct Scenes {
    Gameplay gameplay;
    MainMenu mainMenu;
    TitleScreen titlescreen;
    Credits credits;
    CharacterCreator characterCreator;
};

enum class SCENE_TYPES : uint8_t {
    NONE,
    TITLESCREEN,
    MAINMENU,
    GAME,
    CREATOR,
    CREDITS,
};

struct GameData {
    bool running;
    const float* dt;
    float* dt_scaler;
    uint64_t* ticks_total;
    AudioSystem audio;
    SCENE_TYPES scene_current;
    SCENE_TYPES scene_previous;
    Scenes scenes;
    Transition transition;
    EditorData editor_data;
    Input input;
    SpriteLibrary sprites;
    FontAtlas font;
    Tileset* tilesetBuffer;
    Arena* arena_main;
    Arena* arena_levels;
    Arena* arena_entities;
    Arena* arena_images;
    Arena* arena_input;
    Arena* arena_scratch;
    Camera camera;
    ImGuiContext* imGui_context;
};
