#pragma once
#include <string>
#include "Enemy.h"
using namespace std;

enum class RoomType {
    EMPTY,
    ENEMY,
    TRAP,
    CHEST,
    FOUNTAIN,
    SHRINE,
    EXIT
};

struct Room {
    RoomType type    = RoomType::EMPTY;
    bool     visited = false;
    bool     cleared = false;   // подія вже відбулась
    Enemy*   enemy   = nullptr;
    int      trapDmg = 0;
    string   loot;
    string   description;

    Room() = default;
    ~Room() { delete enemy; }

    // заборона копіювання (через сирий pointer)
    Room(const Room&)            = delete;
    Room& operator=(const Room&) = delete;

    // дозволяємо переміщення
    Room(Room&& o) noexcept;
    Room& operator=(Room&& o) noexcept;
};