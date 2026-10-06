#include "charCreator.h"

#include <cassert>

#include "engine/core/common.h"
#include "engine/graphics/rendering.h"
#include "engine/memory/arena.h"
#include "engine/ui/button.h"
#include "engine/graphics/spriteLibrary.h"
#include "game/app/gameState.h"

void Creator::MakeCharacter( Sprite* sprite ) {
}

void Creator::Initialize( CharacterCreator* characterCreator, SpriteLibrary* sprites, FontAtlas* font,
                          Arena* arena_main ) {
    assert(characterCreator->initialized == false);
    characterCreator->buttons_count = 3;
    characterCreator->rect = {50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100};
    characterCreator->buttons = ALLOC_ARRAY(arena_main, Button, characterCreator->buttons_count);
    characterCreator->background = sprites->GetSprite(SPRITE_ID::Button_Basic);
    float part_w = characterCreator->rect.w / 3.0 - 50;

    SetupButton(&characterCreator->buttons[0], sprites, ButtonType::CHARACTER,
                Alignment::Centered,
                {part_w, SCREEN_HEIGHT / 2.0},
                font, "KNIGHT", false, SPRITE_ID::knight_f_idle);

    SetupButton(&characterCreator->buttons[1], sprites, ButtonType::CHARACTER, Alignment::Centered,
                {part_w * 2, SCREEN_HEIGHT / 2.0f},
                font, "WIZZARD", false, SPRITE_ID::wizzard_f_idle);

    SetupButton(&characterCreator->buttons[2], sprites, ButtonType::CHARACTER, Alignment::Centered,
                {part_w * 3, SCREEN_HEIGHT / 2.0f},
                font, "ELF", false, SPRITE_ID::elf_f_idle);
}

void Creator::Update( GameData* data ) {
    CharacterCreator* cc = &data->scenes.characterCreator;
    Input* input = &data->input;
    cc->activeButtonCount = GetActiveButtonCount(cc->buttons, cc->buttons_count);
    if (cc->activeButtonCount == 0) {
        return;
    }
    cc->activeButtons = ALLOC_ARRAY(data->arena_scratch, Button *, cc->activeButtonCount);
    int index = 0;
    for (int i = 0; i < cc->buttons_count; i++) {
        if (cc->buttons[i].active) {
            cc->activeButtons[index++] = &cc->buttons[i];
        }
    }
    int* buttonIndex = &cc->activeButtonIndex;
    bool mouseMoving = input->mouse_magnitude > 0.1;
    if (mouseMoving) {
        for (int i = 0; i < cc->activeButtonCount; i++) {
            Button* button = cc->activeButtons[i];
            if (IsHoveredOver(button, input->mouse_x, input->mouse_y)) {
                *buttonIndex = i;
                break;
            }
        }
    }
    bool up = KeyPressed(input, SDL_SCANCODE_UP);
    bool down = KeyPressed(input, SDL_SCANCODE_DOWN);
    if (up || down) {
        int direction = up ? -1 : 1;
        *buttonIndex += direction + cc->activeButtonCount;
        *buttonIndex = *buttonIndex % cc->activeButtonCount;
    }
    Button* selected = cc->activeButtons[*buttonIndex];
    if (selected != nullptr) {
        if (KeyPressed(input, SDL_SCANCODE_RETURN)) {
            PressButton(data, cc->activeButtons[*buttonIndex]);
            return;
        }
    }
    if (IsHoveredOver(selected, input->mouse_x, input->mouse_y)) {
        if (MousePressed(input, MouseButton::LEFT)) {
            PressButton(data, selected);
        }
    }
}

void Creator::Draw( CharacterCreator* characterCreator, SDL_Renderer* renderer, SpriteLibrary* sprites, Input* input ) {
    float scale = SCREEN_HEIGHT / ((float) characterCreator->background->height * UPSCALE_FACTOR);
    RenderNineSlice(sprites->GetSprite(SPRITE_ID::Button_Basic), characterCreator->rect, renderer, 100);
    for (int i = 0; i < characterCreator->activeButtonCount; i++) {
        Button* button = characterCreator->activeButtons[i];
        RenderButton(button, i == characterCreator->activeButtonIndex, renderer);
    }
    characterCreator->activeButtonCount = 0;
}
