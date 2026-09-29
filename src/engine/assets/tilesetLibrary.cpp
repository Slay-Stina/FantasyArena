#include "engine/assets/tilesetLibrary.h"

#include "engine/memory/arena.h"

#include <cassert>

uint16_t Get_Tileset_ID_Offset_From_Tilemap( int id_limit, const Json::Value& tmj_result ) {
    int highest_tilemap_start_id = 0;
    for (const Json::Value& tileset: tmj_result["tilesets"]) {
        int first_id = tileset["firstgid"].asInt();
        if (first_id <= id_limit && first_id > highest_tilemap_start_id) {
            highest_tilemap_start_id = first_id;
        }
    }
    return highest_tilemap_start_id;
}

uint16_t GetLocalTileID( uint16_t id_global, const Json::Value& tmj_result ) {
    return id_global - Get_Tileset_ID_Offset_From_Tilemap(id_global, tmj_result);
}

namespace {
    constexpr int DUNGEON_TILE_COUNT = 1024;

    const uint16_t DUNGEON_WALKABLE_TILES[] = {
        129, 130, 131, 161, 162, 163, 193, 194, 195,
        262, 294, 385, 386, 387, 388, 389, 417, 418, 419, 420, 421, 422, 882,
    };
}

namespace AssetManagement {
    void LoadAllTilesets( Tileset* tilesetBuffer, Memory::Arena* arena_images ) {
        Tileset* tileset = &tilesetBuffer[(int) TILESETS::Dungeon];
        tileset->type = TILESETS::Dungeon;
        tileset->walkableBuffer = ALLOC_ARRAY(arena_images, bool, DUNGEON_TILE_COUNT);

        for (uint16_t tile: DUNGEON_WALKABLE_TILES) {
            assert(tile < DUNGEON_TILE_COUNT);
            tileset->walkableBuffer[tile] = true;
        }
    }
}
