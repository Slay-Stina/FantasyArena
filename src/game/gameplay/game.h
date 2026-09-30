#pragma once

#include <SDL3/SDL_render.h>

#include "engine/memory/arena.h"
#include "game/gameplay/entity.h"
#include "engine/input/input.h"
#include "game/gameplay/levels.h"

struct GameData;
struct Tileset;

struct Gameplay {
    LevelData* levels;
    int levelCount;
    int currentLevelIndex;
    bool initialized;
    int playerIndex;
    int activePlayerCount;
    Entity* player;
    float level_complete_timer;
};

inline LevelData* GetCurrentLevel( Gameplay* game ) {
    return &game->levels[game->currentLevelIndex];
}

namespace Game {
    void Initialize( Gameplay* gameplay, Arena* arena_levels, Tileset* tilesetBuffer );

    void Update( Gameplay* gameplay, Input* input, Arena* arena_scratch, Arena* arena_entities, float dt );

    void Draw( GameData* data, SDL_Renderer* renderer );

    void StartLevel( Gameplay* gameplay, Arena* arena_entities );
}
