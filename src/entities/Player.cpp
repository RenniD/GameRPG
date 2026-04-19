#include "Player.h"
#include <iostream>
#include <algorithm>
using namespace std;

Player::Player(string n)
    : name(n), hp(100), maxHp(100),
      attack(18), defense(5),
      x(0), y(0),
      level(1), exp(0), gold(0) {}

bool Player::isAlive() const { return hp > 0; }

void Player::takeDamage(int dmg) {
    int actual = max(1, dmg - defense);
    hp -= actual;
    if (hp < 0) hp = 0;
}

void Player::heal(int amount) {
    hp = min(maxHp, hp + amount);
}

void Player::gainExp(int amount) {
    exp += amount;
    if (exp >= level * 100) {
        exp -= level * 100;
        level++;
        maxHp   += 20;
        hp       = min(hp + 20, maxHp);
        attack  += 3;
        defense += 1;
        cout << "\n  *** РІВЕНЬ ВИЩЕ! Тепер " << level << " рівень! ***\n";
        cout << "  MaxHP: " << maxHp
             << "  ATK: " << attack
             << "  DEF: " << defense << "\n";
    }
}

void Player::addItem(const Item& item) {
    items.push_back(item);
}

bool Player::hasItem(const string& n) const {
    for (const auto& i : items)
        if (i.name == n) return true;
    return false;
}

void Player::printStatus() const {
    cout << "  " << name
         << " | Рівень: " << level
         << " | HP: " << hp << "/" << maxHp
         << " | ATK: " << attack
         << " | DEF: " << defense
         << " | Золото: " << gold
         << " | EXP: " << exp << "/" << level * 100
         << "\n";
}

void Player::printInventory() const {
    cout << "\n  ── Інвентар ──\n";
    if (items.empty()) {
        cout << "  (порожньо)\n";
        return;
    }
    for (size_t i = 0; i < items.size(); i++) {
        cout << "  [" << (i+1) << "] "
             << items[i].typeLabel() << " "
             << items[i].name
             << "  — " << items[i].description << "\n";
    }
}