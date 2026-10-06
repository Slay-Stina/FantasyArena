#pragma once
#include <SDL3/SDL_rect.h>

#include "engine/graphics/FontAtlas.h"
#include "engine/graphics/spriteLibrary.h"

struct FontAtlas;
enum class Alignment;
struct GameData;

enum class ButtonType {
    NONE, START_GAME, CREDITS, CHARACTER, QUIT
};

struct Button {
    ButtonType type;
    Alignment mode;
    SDL_FRect rect;
    Sprite* sprite;
    bool active;
    bool dynamic;
    FontAtlas* font;
    const char* text;
};

void PressButton( GameData* data, Button* button );

int GetActiveButtonCount( Button* buttons, int count );

bool IsHoveredOver( Button* button, float x, float y );

void SetupButton( Button* button, SpriteLibrary* sprites, ButtonType type, Alignment mode, SDL_FRect rect,
                  FontAtlas* font = nullptr,
                  const char* text = nullptr,
                  bool dynamic = false,
                  SPRITE_ID id = SPRITE_ID::Button_Basic );

void FitButtonToText( Button* button, float padding );
