#include "engine/input/input.h"

#include <cassert>

bool AnyKeyPressed( const Input* input ) {
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (KeyPressed(input, (SDL_Scancode) i)) {
            return true;
        }
    }
    return false;
}

bool KeyPressed( const Input* input, SDL_Scancode key ) {
    if (input->keys_previous == nullptr) {
        return input->keys_current[key];
    }
    return input->keys_current[key] && !input->keys_previous[key];
}

bool KeyHeld( const Input* input, SDL_Scancode key ) {
    if (input->keys_previous == nullptr) {
        return false;
    }
    return input->keys_current[key] && input->keys_previous[key];
}

bool KeyReleased( const Input* input, SDL_Scancode key ) {
    if (input->keys_previous == nullptr) {
        return false;
    }
    return !input->keys_current[key] && input->keys_previous[key];
}

bool KeyHeld_ForTime( const Input* input, SDL_Scancode key, float min_length ) {
    return input->keys_held_time[key] >= min_length;
}

void UpdateKeys( Input* input, float dt ) {
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (input->keys_current[i]) {
            input->keys_held_time[i] += dt;
        } else {
            input->keys_held_time[i] = 0;
        }
    }
    memcpy((void*) input->keys_previous, input->keys_current, SDL_SCANCODE_COUNT * sizeof(bool));
}

void ResetKeyHeldTime( Input* input, SDL_Scancode key ) {
    input->keys_held_time[key] = 0;
}

void ResetAll( Input* input ) {
    memset((void*) input->keys_current, 0, sizeof(bool) * SDL_SCANCODE_COUNT);
    memset((void*) input->keys_previous, 0, sizeof(bool) * SDL_SCANCODE_COUNT);
    memset(input->keys_held_time, 0, sizeof(float) * SDL_SCANCODE_COUNT);
}

SDL_MouseButtonFlags ButtonToFlag( MouseButton button ) {
    switch (button) {
        case MouseButton::LEFT:
            return SDL_BUTTON_LMASK;
        case MouseButton::MIDDLE:
            return SDL_BUTTON_MMASK;
        case MouseButton::RIGHT:
            return SDL_BUTTON_RMASK;
            break;
        case MouseButton::COUNT:
            assert(false);
            break;
    }
}

bool MousePressed( const Input* input, MouseButton button ) {
    SDL_MouseButtonFlags flag = ButtonToFlag(button);
    return (input->mouse_current & flag) != 0 && (input->mouse_previous & flag) == 0;
}

bool MouseReleased( const Input* input, MouseButton button ) {
    SDL_MouseButtonFlags flag = ButtonToFlag(button);
    return (input->mouse_current & flag) == 0 && (input->mouse_previous & flag) != 0;
}

bool MouseHeld( const Input* input, MouseButton button ) {
    SDL_MouseButtonFlags flag = ButtonToFlag(button);
    return (input->mouse_current & flag) != 0 && (input->mouse_previous & flag) != 0;
}

bool MouseHeld_ForTime( const Input* input, MouseButton button, float min_length ) {
    SDL_MouseButtonFlags flag = ButtonToFlag(button);
    return input->mouse_held_time[flag] >= min_length;
}

void UpdateMouse( Input* input, float dt ) {
    if (MouseHeld(input, MouseButton::LEFT)) {
        input->mouse_held_time[(int) MouseButton::LEFT] += dt;
    } else {
        input->mouse_held_time[(int) MouseButton::LEFT] = 0;
    }
    if (MouseHeld(input, MouseButton::MIDDLE)) {
        input->mouse_held_time[(int) MouseButton::MIDDLE] += dt;
    } else {
        input->mouse_held_time[(int) MouseButton::MIDDLE] = 0;
    }
    if (MouseHeld(input, MouseButton::RIGHT)) {
        input->mouse_held_time[(int) MouseButton::RIGHT] += dt;
    } else {
        input->mouse_held_time[(int) MouseButton::RIGHT] = 0;
    }
    input->mouse_previous = input->mouse_current;
}
