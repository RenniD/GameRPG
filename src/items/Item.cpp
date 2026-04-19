#include "Item.h"

Item::Item(string n, string d, ItemType t, int v, int stat, int hp)
    : name(n), description(d), type(t),
      value(v), statBonus(stat), hpBonus(hp) {}

string Item::typeLabel() const {
    switch (type) {
        case ItemType::CONSUMABLE: return "[Зілля]   ";
        case ItemType::WEAPON:     return "[Зброя]   ";
        case ItemType::ARMOR:      return "[Броня]   ";
        case ItemType::ACCESSORY:  return "[Аксесуар]";
        default:                   return "[?]       ";
    }
}