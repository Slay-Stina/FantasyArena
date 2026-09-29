#pragma once

#include <SDL3/SDL_render.h>

#include "engine/graphics/FontAtlas.h"

struct Credits {
    static constexpr int MAX_LINES = 64;
    static constexpr int MAX_LINE_LENGTH = 256;

    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int line_count;
    float scroll;
    FontAtlas font;
};

namespace CreditsScreen {
    void Initialize( Credits* credits, SDL_Renderer* renderer, const char* path );
    void Reload( Credits* credits );
    void Update( Credits* credits, float dt );
    void Draw( Credits* credits, SDL_Renderer* renderer, FontAtlas* title_font );
}
