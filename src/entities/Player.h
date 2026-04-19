#pragma once
#include <string>
#include <vector>
#include "Item.h"
using namespace std;

class Player {
public:
    string       name;
    int          hp, maxHp;
    int          attack;
    int          defense;
    int          x, y;
    int          level;
    int          exp;
    int          gold;
    vector<Item> items;       // повноцінний інвентар предметів

    Player(string name);

    bool isAlive()              const;
    void takeDamage(int dmg);
    void heal(int amount);
    void gainExp(int amount);
    void addItem(const Item& item);
    bool hasItem(const string& name) const;
    void printStatus()          const;
    void printInventory()       const;
};