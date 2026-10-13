#include "engine/ui/button.h"

#include <cassert>

#include "engine/physics/collision.h"
#include "engine/core/common.h"
#include "game/app/core.h"
#include "game/app/gameState.h"
#include "engine/graphics/rendering.h"

void PressButton( GameData* data, Button* button ) {
    if (button == nullptr)
        return;
    assert(button->active);
    switch (button->type) {
        case ButtonType::START_GAME:
            ChangeScene(data, SCENE_TYPES::CREATOR);
            break;
        case ButtonType::CREDITS:
            ChangeScene(data, SCENE_TYPES::CREDITS);
            break;
        case ButtonType::QUIT:
            data->running = false;
            break;
        case ButtonType::CHARACTER:
            printf("\n %s", button->text);
            Creator::MakeCharacter(button->sprite);
            break;
        case ButtonType::NONE:
            assert(false);
            break;
    }
}

int GetActiveButtonCount( Button* buttons, int count ) {
    if (count == 0) {
        return 0;
    }
    int activeButtons = 0;
    for (int i = 0; i < count; i++) {
        if (buttons[i].active)
            activeButtons++;
    }
    return activeButtons;
}

bool IsHoveredOver( Button* button, float x, float y ) {
    if (button == nullptr || !button->active)
        return false;
    assert(button->type != ButtonType::NONE);
    return CheckCollisionInsideBounds(button->rect, x, y);
}

void SetupButton( Button* button, SpriteLibrary* sprites, ButtonType type, Alignment mode, SDL_FRect rect,
                  FontAtlas* font, const char* text, bool dynamic, SPRITE_ID id ) {
    assert(type != ButtonType::NONE);
    button->mode = mode;
    button->dynamic = dynamic;
    button->type = type;
    button->rect = rect;

    bool hasText = !IsStringEmpty(text);
    if (font == nullptr) {
        assert(!hasText);
    }
    if (hasText) {
        assert(font != nullptr);
        button->font = font;
        button->text = text;
    }

    switch (button->type) {
        case ButtonType::QUIT:
        case ButtonType::CREDITS:
        case ButtonType::CHARACTER:
        case ButtonType::START_GAME:
            button->sprite = sprites->GetSprite(id);
            break;
        default:
            button->sprite = sprites->GetSprite(SPRITE_ID::Fallback);
            break;
    }

    if (button->dynamic) {
        FitButtonToText(button, 25);
    } else if (button->type == ButtonType::CHARACTER) {
        SDL_FRect frame = {0, 0, (float) button->sprite->width, (float) button->sprite->height};
        if (button->sprite->frames != nullptr && button->sprite->frame_count > 0) {
            frame = button->sprite->frames[0];
        }
        button->rect.w = frame.w * CHARACTER_SPRITE_SCALE;
        button->rect.h = frame.h * CHARACTER_SPRITE_SCALE;
    }
    if (button->mode == Alignment::Centered) {
        button->rect.x -= button->rect.w / 2;
        button->rect.y -= button->rect.h / 2;
    }
    button->active = true;
}

void FitButtonToText( Button* button, float padding ) {
    if (IsStringEmpty(button->text)) return;
    int w = 0, h = 0;
    for (int i = 0; i < (int) SDL_strlen(button->text); i++) {
        Glyph glyph = button->font->GetGlyph(button->text[i]);
        w += glyph.atlasPosition.w;
        h = h > glyph.atlasPosition.h ? h : glyph.atlasPosition.h;
    }
    button->rect.w = w + padding * 2;
    button->rect.h = h + padding * 2;
}
