#include "game/editor/levelEditor.h"
#include "game/gameplay/levels.h"

#include <imgui.h>

#include "game/gameplay/command.h"
#include "engine/graphics/rendering.h"

namespace {
    bool SpriteButton( const char* id, SpriteRenderInfo info, ImVec2 size ) {
        Sprite* sprite = info.sprite;
        if (sprite == nullptr || sprite->texture == nullptr) {
            return false;
        }
        ImVec2 uv0;
        ImVec2 uv1;
        if (sprite->frames != nullptr && sprite->frame_count > 0) {
            SDL_FRect frame = sprite->frames[info.frame % sprite->frame_count];
            uv0 = ImVec2(frame.x / sprite->width, frame.y / sprite->height);
            uv1 = ImVec2((frame.x + frame.w) / sprite->width, (frame.y + frame.h) / sprite->height);
        } else {
            int count_x = sprite->sprite_count_x == NOT_SET ? 1 : sprite->sprite_count_x;
            int count_y = sprite->sprite_count_y == NOT_SET ? 1 : sprite->sprite_count_y;
            uv0 = ImVec2((float) (info.frame % count_x) / count_x, (float) (info.frame / count_x) / count_y);
            uv1 = ImVec2(uv0.x + 1.0f / count_x, uv0.y + 1.0f / count_y);
        }
        return ImGui::ImageButton(id, sprite->texture, size, uv0, uv1);
    }

    const char* ENTITY_NAMES[(int) ENTITY_ID::COUNT] = {
        "Knight", "Wizzard", "Elf", "Dwarf", "Lizard", "Ogre",
        "Necromancer", "Goblin", "Imp", "Chort", "Crate"
    };
}

namespace EDITOR {
    void DrawObjectPanel( Editor* editor, SpriteLibrary& sprites ) {
        ImGui::Begin("objects");
        ImVec2 size = {32, 32};
        for (int i = 0; i < (int) ENTITY_ID::COUNT; i++) {
            if (i > 0) {
                ImGui::SameLine();
            }
            if (SpriteButton(ENTITY_NAMES[i], sprites.GetSprite((ENTITY_ID) i), size)) {
                editor->object_to_place_id = (ENTITY_ID) i;
                editor->has_selection = true;
            }
        }
        ImGui::End();
    }

    void PlaceObject( const int x, const int y, Editor* editor, LevelData* level, CommandBuffer* buffer ) {
        if (!editor->has_selection) {
            return;
        }
        AddCommand add(x, y, editor->object_to_place_id);
        Push(buffer, add, level);
    }

    void DrawPreview( Editor* editor, Input* input, SDL_Renderer* renderer, LevelData* level, Camera* camera,
                      SpriteLibrary& sprites ) {
        int x;
        int y;
        camera::WorldToGrid(input->mouse_x, input->mouse_y, &x, &y, level, camera->camera_z);
        SpriteRenderInfo preview = sprites.GetSprite(editor->object_to_place_id);
        if (preview.sprite == nullptr || !editor->has_selection) {
            return;
        }
        RenderSprite_OnTile(preview, level, renderer, camera, x, y, 1, 0.5);
    }

    void Update( Editor* editor, Input* input, LevelData* level, CommandBuffer* buffer, Camera* camera ) {
        float zoom = camera->camera_z;
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        if (MousePressed(input, MouseButton::LEFT)) {
            if (camera::GetIsPointInsideGrid(input->mouse_x, input->mouse_y, level, zoom)) {
                int x;
                int y;
                camera::WorldToGrid(input->mouse_x, input->mouse_y, &x, &y, level, zoom);
                PlaceObject(x, y, editor, level, buffer);
            }
        } else if (MousePressed(input, MouseButton::RIGHT)) {
            if (camera::GetIsPointInsideGrid(input->mouse_x, input->mouse_y, level, zoom)) {
                int x;
                int y;
                camera::WorldToGrid(input->mouse_x, input->mouse_y, &x, &y, level, zoom);
                Entity* entity = GetEntity(level, x, y);
                if (entity == nullptr) {
                    return;
                }
                RemoveCommand remove(entity);
                Push(buffer, remove, level);
            }
        }
    }
}
