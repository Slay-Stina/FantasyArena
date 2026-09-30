#include "game/gameplay/levelRenderer.h"

#include <algorithm>
#include <cmath>

#include "engine/graphics/rendering.h"

void RenderLevel( GameData* gameData, SDL_Renderer* renderer ) {
    Gameplay* gameplay = &gameData->scenes.gameplay;
    LevelData* level = GetCurrentLevel(gameplay);

    Sprite* tileset;
    switch (level->tileset->type) {
        case TILESETS::Dungeon:
            tileset = gameData->sprites.GetSprite(SPRITE_ID::dungeon_tileset);
            break;
        case TILESETS::NONE:
        case TILESETS::COUNT:
            assert(false);
            break;
    }

    for (int x = 0; x < level->w; x++) {
        for (int y = 0; y < level->h; y++) {
            uint16_t id = GetCellID(level, x, y);
            RenderTile(tileset, id, level, renderer, &gameData->camera, x, y, 1, 1);
        }
    }
    for (int i = 0; i < level->goalCount; i++) {
        Goal goal = level->goals[i];
        Sprite* sprite = gameData->sprites.GetSprite(SPRITE_ID::Goal);
        int frame = (int) (goal.blink_timer / 0.2) % (sprite->sprite_count_x * sprite->sprite_count_y);
        RenderSprite_OnTile({frame, sprite}, level, renderer, &gameData->camera, goal.x, goal.y);
    }
}

bool IsEntityBelowOtherEntity( Entity* a, Entity* b ) {
    return a->position.y < b->position.y;
}

void RenderEntities( GameData* data, SDL_Renderer* renderer ) {
    Gameplay* gameplay = &data->scenes.gameplay;
    LevelData* lvl = GetCurrentLevel(gameplay);
    Entity** sortedEntities = ALLOC_ARRAY(data->arena_scratch, Entity *, lvl->entityCount);

    for (int i = 0; i < lvl->entityCount; i++) {
        sortedEntities[i] = &lvl->entityBuffer[i];
    }
    sort(sortedEntities, sortedEntities + lvl->entityCount, IsEntityBelowOtherEntity);

    for (int i = 0; i < lvl->entityCount; i++) {
        Entity* entity = sortedEntities[i];
        if (!entity->active) {
            continue;
        }
        SpriteRenderInfo sprite = data->sprites.GetSprite_FromEntityState(entity, data->ticks_total);
        float world_x = entity->position.x;
        float world_y = entity->position.y;
        camera::GridToWorld(&world_x, &world_y, lvl, data->camera.camera_z);
        Sprite* dropshadow = data->sprites.GetSprite(SPRITE_ID::Dropshadow);
        RenderSprite_World(dropshadow, renderer, &data->camera, world_x, world_y, 1, 0.4, false);
        RenderSprite_World(sprite, renderer, &data->camera, world_x, world_y, 1, 1, sprite.flipped);
    }
}
