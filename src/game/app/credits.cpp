#include "game/app/credits.h"

#include <cstdio>
#include <fstream>
#include <string>

#include "engine/core/common.h"
#include "engine/graphics/rendering.h"

namespace {
    float Measure( const FontAtlas& font, const char* text ) {
        float width = 0;
        for (int i = 0; text[i] != '\0'; i++) {
            width += font.GetGlyph(text[i]).atlasPosition.w;
        }
        return width;
    }

    void FitLine( char* out, const char* in, const FontAtlas& font, float max_width ) {
        const float ellipsis_width = Measure(font, "...");
        const int capacity = Credits::MAX_LINE_LENGTH - 1;

        float width = 0;
        int length = 0;
        for (int i = 0; in[i] != '\0' && length < capacity; i++) {
            float glyph_width = font.GetGlyph(in[i]).atlasPosition.w;
            if (width + glyph_width + ellipsis_width > max_width) {
                break;
            }
            out[length++] = in[i];
            width += glyph_width;
        }

        if (in[length] != '\0') {
            if (length > capacity - 3) {
                length = capacity - 3;
            }
            out[length++] = '.';
            out[length++] = '.';
            out[length++] = '.';
        }
        out[length] = '\0';
    }

    bool ParseLine( const std::string& raw, char* out ) {
        const size_t first = raw.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            return false;
        }
        const size_t last = raw.find_last_not_of(" \t\r\n");
        std::string line = raw.substr(first, last - first + 1);

        size_t pos = 0;
        while (pos < line.size() && line[pos] == '#') {
            pos++;
        }
        while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) {
            pos++;
        }
        if (pos + 1 < line.size() && (line[pos] == '-' || line[pos] == '*') && line[pos + 1] == ' ') {
            pos += 2;
        }
        line = line.substr(pos);

        const size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) {
            return false;
        }
        const size_t end = line.find_last_not_of(" \t");
        line = line.substr(start, end - start + 1);

        if (line.empty() || line == "---") {
            return false;
        }

        snprintf(out, Credits::MAX_LINE_LENGTH, "%s", line.c_str());
        return true;
    }

    void ParseFile( Credits* credits, const char* path ) {
        credits->line_count = 0;
        credits->scroll = 0;

        const float max_width = SCREEN_WIDTH - 80.0f;
        char parsed[Credits::MAX_LINE_LENGTH];

        std::ifstream file(path);
        if (!file.is_open()) {
            snprintf(credits->lines[credits->line_count++], Credits::MAX_LINE_LENGTH, "%s",
                     "Missing assets/audio/CREDITS.md");
            return;
        }

        std::string raw;
        while (std::getline(file, raw) && credits->line_count < Credits::MAX_LINES) {
            if (!ParseLine(raw, parsed)) {
                continue;
            }
            FitLine(credits->lines[credits->line_count], parsed, credits->font, max_width);
            credits->line_count++;
        }
    }
}

void CreditsScreen::Initialize( Credits* credits, SDL_Renderer* renderer, const char* path ) {
    credits->font.LoadFont(renderer, "assets/fonts/ByteBounce.ttf", 20);
    ParseFile(credits, path);
}

void CreditsScreen::Reload( Credits* credits ) {
    ParseFile(credits, "assets/audio/CREDITS.md");
}

void CreditsScreen::Update( Credits* credits, float dt ) {
    const float top = 120.0f;
    const float bottom = 500.0f;
    const float line_height = credits->font.GetGlyph('H').atlasPosition.h + 6.0f;
    const float total_height = credits->line_count * line_height;
    const float max_scroll = total_height > (bottom - top) ? total_height - (bottom - top) : 0;

    if (max_scroll > 0) {
        credits->scroll += dt * 20.0f;
        if (credits->scroll > max_scroll) {
            credits->scroll = max_scroll;
        }
    }
}

void CreditsScreen::Draw( Credits* credits, SDL_Renderer* renderer, FontAtlas* title_font ) {
    const float center_x = SCREEN_WIDTH / 2.0f;
    const float top = 120.0f;
    const float bottom = 500.0f;
    const float line_height = credits->font.GetGlyph('H').atlasPosition.h + 6.0f;
    const float total_height = credits->line_count * line_height;
    const float available = bottom - top;

    RenderText(title_font, "Credits", renderer, nullptr, center_x, 40.0f, Alignment::Centered);

    float start_y = top;
    if (total_height < available) {
        start_y = top + (available - total_height) / 2.0f;
    } else {
        start_y = top - credits->scroll;
    }

    for (int i = 0; i < credits->line_count; i++) {
        RenderText(&credits->font, credits->lines[i], renderer, nullptr, center_x, start_y + i * line_height,
                   Alignment::Centered);
    }

    RenderText(&credits->font, "Press any key to return", renderer, nullptr, center_x, SCREEN_HEIGHT - 30.0f,
               Alignment::Centered);
}
