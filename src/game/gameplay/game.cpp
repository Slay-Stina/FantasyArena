#include "game/gameplay/game.h"

#include <cmath>

#include <SDL3/SDL_scancode.h>

#include "engine/core/common.h"
#include "game/gameplay/levelRenderer.h"
#include "game/gameplay/levels.h"

namespace {
    const char* LEVEL_PATHS[] = {
        "assets/levels/template.tmj",
    };
    constexpr int LEVEL_COUNT = (int) (sizeof(LEVEL_PATHS) / sizeof(LEVEL_PATHS[0]));
    static_assert(LEVEL_COUNT <= MAX_LEVELS);

    bool IsBlocked( LevelData* level, float x, float y ) {
        const float half = 0.5f;
        int x0 = (int) floorf(x - half);
        int x1 = (int) floorf(x + half - 0.0001f);
        int y0 = (int) floorf(y - half);
        int y1 = (int) floorf(y + half - 0.0001f);
        for (int cy = y0; cy <= y1; cy++) {
            for (int cx = x0; cx <= x1; cx++) {
                if (cx < 0 || cy < 0 || cx >= level->w || cy >= level->h) {
                    return true;
                }
                if (!IsWalkable(cx, cy, level)) {
                    return true;
                }
            }
        }
        return false;
    }

    void MoveWithCollision( Entity* entity, LevelData* level, float dx, float dy ) {
        if (!IsBlocked(level, entity->position.x + dx, entity->position.y)) {
            entity->position.x += dx;
        }
        if (!IsBlocked(level, entity->position.x, entity->position.y + dy)) {
            entity->position.y += dy;
        }
    }
}

void Game::Initialize( Gameplay* gameplay, Arena* arena_levels, Tileset* tilesetBuffer ) {
    assert(gameplay->initialized == false);
    gameplay->currentLevelIndex = 0;
    gameplay->playerIndex = 0;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        CreateLevel(arena_levels, &gameplay->levels[i], &tilesetBuffer[(int) TILESETS::Dungeon], LEVEL_PATHS[i]);
    }
    gameplay->initialized = true;
}

void Game::Update( Gameplay* gameplay, Input* input, Arena* arena_scratch, Arena* arena_entities,
                   float dt ) {
    if (KeyPressed(input, SDL_SCANCODE_R)) {
        StartLevel(gameplay, arena_entities);
        return;
    }
    LevelData* level = GetCurrentLevel(gameplay);
    Entity* entityBuffer = level->entityBuffer;

    // Check for player and set in gameplay
    gameplay->player = nullptr;
    for (int i = 0; i < level->entityCount; i++) {
        if (entityBuffer[i].active == false) {
            continue;
        }
        if (HasBehaviour(&level->entityBuffer[i], IS_PLAYER)) {
            gameplay->player = &entityBuffer[i];
        }
    }

    Entity* player = gameplay->player;
    if (player == nullptr) {
        return;
    }

    for (int i = 0; i < level->goalCount; i++) {
        Entity* entity = GetEntity(level, level->goals[i].x, level->goals[i].y);
        if (entity != nullptr && !IsActing(entity)) {
            level->goals[i].blink_timer += dt;
        } else {
            level->goals[i].blink_timer = 0;
        }
    }

    if (level->goalCount > 0) {
        int goals_reached = 0;
        for (int i = 0; i < level->goalCount; i++) {
            Goal goal = level->goals[i];
            Entity* entity = GetEntity(level, goal.x, goal.y);
            if (entity == nullptr) {
                break;
            }
            if (!IsActing(entity) && HasBehaviour(entity, IS_PLAYER)) {
                goals_reached++;
            }
        }

        if (goals_reached == level->goalCount) {
            gameplay->level_complete_timer += dt;
            if (gameplay->level_complete_timer >= LEVEL_COMPLETE_DELAY) {
                gameplay->currentLevelIndex++;
                StartLevel(gameplay, arena_entities);
                return;
            }
        } else {
            gameplay->level_complete_timer = 0;
        }
    }

    float dx = 0;
    float dy = 0;
    if (KeyHeld(input, SDL_SCANCODE_RIGHT)) dx += 1;
    if (KeyHeld(input, SDL_SCANCODE_LEFT)) dx -= 1;
    if (KeyHeld(input, SDL_SCANCODE_UP)) dy -= 1;
    if (KeyHeld(input, SDL_SCANCODE_DOWN)) dy += 1;

    if (dx != 0 || dy != 0) {
        if (dx != 0 && dy != 0) {
            const float diagonal = 0.70710678f;
            dx *= diagonal;
            dy *= diagonal;
        }
        player->action = Actions::MOVING;
        if (fabsf(dx) >= fabsf(dy)) {
            player->facing_current = dx > 0 ? Direction::RIGHT : Direction::LEFT;
        } else {
            player->facing_current = dy > 0 ? Direction::DOWN : Direction::UP;
        }
        MoveWithCollision(player, level, dx * MOVE_SPEED * dt, dy * MOVE_SPEED * dt);
    } else {
        player->action = Actions::NONE;
    }
}

void Game::Draw( GameData* data, SDL_Renderer* renderer ) {
    RenderLevel(data, renderer);
    RenderEntities(data, renderer);
}

void Game::StartLevel( Gameplay* gameplay, Arena* arena_entities ) {
    CreateEntities(&gameplay->levels[gameplay->currentLevelIndex], arena_entities);
    gameplay->playerIndex = 0;
    gameplay->level_complete_timer = 0;
}
