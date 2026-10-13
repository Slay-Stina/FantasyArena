#pragma once
#include "engine/graphics/spriteLibrary.h"
#include "engine/graphics/camera.h"
#include "game/gameplay/levels.h"

struct Button;
struct FontAtlas;

enum class Alignment {
    Right,
    Centered
};

void RenderNineSlice( Sprite* sprite, SDL_FRect rect, SDL_Renderer* renderer, uint8_t colorOverlay = 250 );

void RenderText( FontAtlas* atlas, const char* text, SDL_Renderer* renderer, Camera* camera, float x,
                 float y, Alignment alignment );

void RenderButton( Button* button, bool is_selected, SDL_Renderer* renderer );

void RenderButton_Dynamic( Button* button, bool is_selected, SDL_Renderer* renderer );

extern bool g_debugDrawRects;

void DrawDebugRect( SDL_Renderer* renderer, const SDL_FRect& rect );

void RenderTile( Sprite* tileset, int cell_id, LevelData* level, SDL_Renderer* renderer,
                 const Camera* camera, float x, float y, float scale, float alpha );

SDL_FRect SetRenderSize( SDL_FRect tilesetRect, Sprite* sprite, const Camera* camera,
                         float scale, float x, float y );

void RenderSprite_World( SpriteRenderInfo spriteInfo, SDL_Renderer* renderer, const Camera* camera,
                         float x, float y, float scale = 1, float alpha = 1, bool flipped = false );

void RenderSprite_OnTile( SpriteRenderInfo spriteInfo, LevelData* lvl, SDL_Renderer* renderer,
                          const Camera* camera, float x, float y, float scale = 1, float alpha = 1,
                          bool flipped = false );
