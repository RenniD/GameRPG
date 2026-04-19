#pragma once
#include <string>
using namespace std;

class Enemy {
public:
    string name;
    int    hp;
    int    attack;
    int    defense;
    int    expReward;
    int    goldReward;
    string type;   // "weak" | "normal" | "elite" | "boss"

    Enemy(string name, int hp, int atk, int def,
          int exp, int gold, string type);

    bool isAlive()           const;
    int  takeDamage(int dmg);
    void printStatus()       const;
};