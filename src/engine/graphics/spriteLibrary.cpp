#include "engine/graphics/spriteLibrary.h"
#include <SDL3_image/SDL_image.h>
#include <cassert>

#include "engine/core/common.h"

namespace {
    const char* FALLBACK_PATH = "assets/sprites/fallback.png";

    struct SpriteDataEntry {
        SPRITE_ID id;
        const char* path;
        int pivot_x = NOT_SET;
        int pivot_y = NOT_SET;
        int sprite_count_x = NOT_SET;
        int sprite_count_y = NOT_SET;
        int framerate = NOT_SET;
    };

    const SpriteDataEntry ALL_SPRITE_DATA[] = {
        {SPRITE_ID::Fallback, FALLBACK_PATH, 8, 8},
        {SPRITE_ID::Dropshadow, "assets/sprites/dropshadow.png", 8, 8},
        {SPRITE_ID::black_1x1, "assets/sprites/1x1_black.png", 0, 0},
        {SPRITE_ID::titlescreen_background, "assets/sprites/titlescreen.png", 325, 200},
        {SPRITE_ID::dungeon_tileset, "assets/sprites/0x72_DungeonTilesetII_v1.7.png", 0, 0, 32, 32},
        {SPRITE_ID::Goal, "assets/sprites/goal.png", 8, 8, 8, 1},
        {SPRITE_ID::Menu_Horizon, "assets/sprites/mainmenu_background.png"},
        {SPRITE_ID::Menu_Cloud_Back, "assets/sprites/mainmenu_cloud_back.png"},
        {SPRITE_ID::Menu_Cloud_Front, "assets/sprites/mainmenu_cloud_front.png"},
        {SPRITE_ID::Menu_Middle, "assets/sprites/mainmenu_middle.png"},
        {SPRITE_ID::Menu_Front, "assets/sprites/mainmenu_front.png"},
        {SPRITE_ID::Button_Basic, "assets/sprites/basic_button.png", 0, 0, 3, 3},
    };

    void LoadOne( Sprite* sprite, const SpriteDataEntry& entry, SDL_Renderer* renderer ) {
        SDL_Surface* surface = IMG_Load(entry.path);
        if (surface == nullptr)
            surface = IMG_Load(FALLBACK_PATH);
        assert(surface != nullptr);
        sprite->texture = SDL_CreateTextureFromSurface(renderer, surface);
        sprite->width = sprite->texture->w;
        sprite->height = sprite->texture->h;
        sprite->sprite_count_x = entry.sprite_count_x;
        sprite->sprite_count_y = entry.sprite_count_y;
        sprite->framerate = entry.framerate;
        if (entry.pivot_x == NOT_SET || entry.pivot_y == NOT_SET) {
            sprite->pivot_x = sprite->width / 2;
            sprite->pivot_y = sprite->height / 2;
        } else {
            sprite->pivot_x = entry.pivot_x;
            sprite->pivot_y = entry.pivot_y;
        }
        SDL_DestroySurface(surface);
    }
}

void SpriteLibrary::LoadAll( SDL_Renderer* renderer, Memory::Arena* arena ) {
    for (const SpriteDataEntry& entry: ALL_SPRITE_DATA) {
        Sprite* sprite = ALLOC(arena, Sprite);
        LoadOne(sprite, entry, renderer);
        sprites[(int) entry.id] = sprite;
    }
    LoadDungeonSprites(arena);
}

namespace {
    struct EntitySprites {
        SPRITE_ID idle;
        SPRITE_ID run;
    };

    const EntitySprites ENTITY_SPRITES[(int) ENTITY_ID::COUNT] = {
        {SPRITE_ID::knight_m_idle, SPRITE_ID::knight_m_run},
        {SPRITE_ID::wizzard_f_idle, SPRITE_ID::wizzard_f_run},
        {SPRITE_ID::elf_m_idle, SPRITE_ID::elf_m_run},
        {SPRITE_ID::dwarf_m_idle, SPRITE_ID::dwarf_m_run},
        {SPRITE_ID::lizard_m_idle, SPRITE_ID::lizard_m_run},
        {SPRITE_ID::ogre_idle, SPRITE_ID::ogre_run},
        {SPRITE_ID::necromancer, SPRITE_ID::necromancer},
        {SPRITE_ID::goblin_idle, SPRITE_ID::goblin_run},
        {SPRITE_ID::imp_idle, SPRITE_ID::imp_run},
        {SPRITE_ID::chort_idle, SPRITE_ID::chort_run},
        {SPRITE_ID::crate, SPRITE_ID::crate},
    };
}

SpriteRenderInfo SpriteLibrary::GetSprite( ENTITY_ID id ) const {
    const EntitySprites& visuals = ENTITY_SPRITES[(int) id];
    return {0, GetSprite(visuals.idle)};
}

SpriteRenderInfo SpriteLibrary::GetSprite_FromEntityState( const Entity* entity, const uint64_t* ticks_total ) const {
    const EntitySprites& visuals = ENTITY_SPRITES[(int) entity->id];
    const bool moving = entity->action == Actions::MOVING && !HasBehaviour(entity, IS_PETRIFIED);
    Sprite* sprite = GetSprite(moving ? visuals.run : visuals.idle);
    const int count = GetSpriteCount(sprite);
    constexpr int ANIMATION_FPS = 8;
    const int frame = count > 1 ? (int) ((*ticks_total * ANIMATION_FPS / TARGET_FPS) % count) : 0;
    const bool flipped = entity->facing_current == Direction::LEFT;
    return {frame, sprite, flipped};
}

Sprite* SpriteLibrary::GetSprite( SPRITE_ID id ) const {
    Sprite* s = sprites[(int) id];
    return s && s->texture ? s : sprites[(int) SPRITE_ID::Fallback];
}

void SpriteLibrary::LoadDungeonSprites( Memory::Arena* arena ) {
    Sprite* atlas = sprites[(int) SPRITE_ID::dungeon_tileset];
    if (atlas == nullptr || atlas->texture == nullptr) {
        return;
    }

    const int first = (int) SPRITE_ID::COUNT - DUNGEON_SPRITE_COUNT;
    for (int i = 0; i < DUNGEON_SPRITE_COUNT; i++) {
        const DungeonSpriteEntry& entry = DUNGEON_SPRITE_TABLE[i];

        Sprite* sprite = ALLOC(arena, Sprite);
        sprite->texture = atlas->texture;
        sprite->width = atlas->width;
        sprite->height = atlas->height;
        sprite->sprite_count_x = NOT_SET;
        sprite->sprite_count_y = NOT_SET;
        sprite->framerate = NOT_SET;
        sprite->frame_count = entry.frame_count;
        sprite->frames = ALLOC_ARRAY(arena, SDL_FRect, entry.frame_count);
        for (int frame = 0; frame < entry.frame_count; frame++) {
            sprite->frames[frame] = entry.frames[frame];
        }
        sprite->pivot_x = entry.pivot_x;
        sprite->pivot_y = entry.pivot_y;

        sprites[first + i] = sprite;
    }
}
