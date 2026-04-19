#pragma once
#include "Player.h"
#include "Room.h"

// Обробка події у кімнаті. Повертає false якщо гравець загинув.
bool handleRoom(Player& player, Room& room);