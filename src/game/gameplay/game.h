#pragma once

#include <SDL3/SDL_render.h>

#include "engine/memory/arena.h"
#include "game/gameplay/entity.h"
#include "engine/input/input.h"
#include "game/gameplay/levels.h"

struct GameData;
struct Tileset;

struct Gameplay {
    CommandBuffer* commandBuffer;
    LevelData* levels;
    int levelCount;
    int currentLevelIndex;
    Position* input_buffer;
    int input_buffer_capacity;
    int input_buffer_write_count;
    int input_buffer_read_count;
    bool initialized;
    int activePlayerIndex;
    int activePlayerCount;
    Entity** activePlayerBuffer;
    float level_complete_timer;
};

inline LevelData* GetCurrentLevel( Gameplay* game ) {
    return &game->levels[game->currentLevelIndex];
}

inline Entity* GetActiveEntity( Gameplay* game ) {
    return game->activePlayerBuffer[game->activePlayerIndex];
}

namespace Game {
    void Initialize(Gameplay * gameplay, Arena * arena_levels, Tileset * tilesetBuffer);

    void Update( Gameplay* gameplay, Input* input, Arena* arena_scratch, Arena* arena_commands, Arena* arena_entities,
                 float dt );

    void Draw(GameData * data, SDL_Renderer * renderer);

    void StartLevel(Gameplay * gameplay, Arena * arena_commands, Arena * arena_entities);
}
