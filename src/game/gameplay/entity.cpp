#include <cassert>

#include "game/gameplay/entity.h"

bool IsActing( Entity* e ) {
    if (e->active == false)
        return false;
    return e->action != Actions::NONE;
}

bool HasBehaviour( const Entity* entity, Behaviour flags ) {
    return (entity->behaviour & flags) == flags;
}

void InitializeBaseBehaviour( Entity* entity ) {
    assert(entity->active);
    switch (entity->id) {
        case ENTITY_ID::KNIGHT:
        case ENTITY_ID::WIZZARD:
        case ENTITY_ID::ELF:
        case ENTITY_ID::DWARF:
            SetBehaviour(entity, (Behaviour) (CAN_MOVE | IS_PLAYER | RESPOND_TO_INPUT));
            break;
        case ENTITY_ID::CRATE:
        default:
            SetBehaviour(entity, CAN_MOVE);
            break;
    }
}

void SetBehaviour( Entity* entity, Behaviour flags ) {
    entity->behaviour = flags;
}

void AddBehaviour( Entity* entity, Behaviour flags ) {
    entity->behaviour = (Behaviour) (entity->behaviour | flags);
}

void RemoveBehaviour( Entity* entity, Behaviour flags ) {
    entity->behaviour = (Behaviour) (entity->behaviour & ~flags);
}
