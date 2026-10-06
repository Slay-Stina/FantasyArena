#pragma once
#include <SDL3/SDL_render.h>

#include "engine/graphics/spriteLibrary.h"

struct Gameplay;
struct Button;
struct FontAtlas;

namespace Memory {
    struct Arena;
}

struct Input;
struct GameData;

struct CharacterCreator {
    bool isActive;
    Button* buttons;
    int buttons_count;
    int activeButtonIndex;
    Button** activeButtons;
    int activeButtonCount;
    bool initialized;
    Sprite* background;
    SDL_FRect rect;
};

namespace Creator {
    void MakeCharacter( Sprite* sprite );

    void Initialize( CharacterCreator* characterCreator, SpriteLibrary* sprites, FontAtlas* font,
                     Memory::Arena* arena_main );

    void Update( GameData* data );

    void Draw( CharacterCreator* characterCreator, SDL_Renderer* renderer, SpriteLibrary* sprites, Input* input );
}
