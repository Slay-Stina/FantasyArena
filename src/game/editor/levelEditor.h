#pragma once
#include "engine/graphics/camera.h"
#include "engine/input/input.h"
#include "engine/graphics/spriteLibrary.h"

struct Editor {
    ENTITY_ID object_to_place_id;
    bool has_selection = false;
};

namespace EDITOR {
    void DrawObjectPanel( Editor* editor, SpriteLibrary& sprites );

    void PlaceObject( int x, int y, Editor* editor, LevelData* level );

    void Update( Editor* editor, Input* input, LevelData* level, Camera* camera );

    void DrawPreview( Editor* editor, Input* input, SDL_Renderer* renderer, LevelData* level, Camera* camera,
                      SpriteLibrary& sprites );
}
