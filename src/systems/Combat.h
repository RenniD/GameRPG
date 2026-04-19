#pragma once
#include "Player.h"
#include "Enemy.h"

// Повертає true якщо гравець переміг, false якщо втік або загинув
bool runCombat(Player& player, Enemy& enemy);