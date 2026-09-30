#pragma once
#include <cassert>
#include <cstdint>

#include "imgui.h"


enum Behaviour : uint32_t {
    NONE = 0,
    CAN_MOVE = 1 << 0,
    IS_PLAYER = 1 << 1,
    RESPOND_TO_INPUT = 1 << 2
};

enum class ENTITY_ID : uint8_t {
    KNIGHT = 0,
    WIZZARD = 1,
    ELF = 2,
    DWARF = 3,
    LIZARD = 4,
    OGRE = 5,
    NECROMANCER = 6,
    GOBLIN = 7,
    IMP = 8,
    CHORT = 9,
    CRATE = 10,
    COUNT
};

enum class Direction {
    DOWN,
    RIGHT,
    LEFT,
    UP
};

struct Vec2 {
    float x;
    float y;

    Vec2& operator+=( const Vec2& other ) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

enum class Actions {
    NONE = 0,
    MOVING = 1
};


struct Entity {
    ENTITY_ID id;
    bool active;
    Vec2 position;
    Actions action;
    Behaviour behaviour;
    Direction facing_current;
};

bool IsActing( Entity* e );

bool HasBehaviour( const Entity* entity, Behaviour flags );

void InitializeBaseBehaviour( Entity* entity );

void SetBehaviour( Entity* entity, Behaviour flags );

void AddBehaviour( Entity* entity, Behaviour flags );

void RemoveBehaviour( Entity* entity, Behaviour flags );
