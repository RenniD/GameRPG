#include "Room.h"
#include <utility>
using namespace std;

Room::Room(Room&& o) noexcept
    : type(o.type), visited(o.visited), cleared(o.cleared),
      enemy(o.enemy), trapDmg(o.trapDmg),
      loot(move(o.loot)), description(move(o.description))
{
    o.enemy = nullptr;
}

Room& Room::operator=(Room&& o) noexcept {
    if (this != &o) {
        delete enemy;
        type        = o.type;
        visited     = o.visited;
        cleared     = o.cleared;
        enemy       = o.enemy;
        trapDmg     = o.trapDmg;
        loot        = move(o.loot);
        description = move(o.description);
        o.enemy     = nullptr;
    }
    return *this;
}