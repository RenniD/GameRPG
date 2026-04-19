#pragma once
#include <string>
using namespace std;

enum class ItemType {
    CONSUMABLE,   // зілля — одноразові
    WEAPON,       // зброя — постійний бонус атаки
    ARMOR,        // броня — постійний бонус захисту
    ACCESSORY     // аксесуар — особливий ефект
};

struct Item {
    string   name;
    string   description;
    ItemType type;
    int      value;      // золото
    int      statBonus;  // ATK або DEF бонус
    int      hpBonus;    // HP відновлення (для зілля)

    Item(string name, string desc, ItemType type,
         int value, int stat = 0, int hp = 0);

    string typeLabel() const;
};