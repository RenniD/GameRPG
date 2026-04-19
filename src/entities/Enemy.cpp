#include "Enemy.h"
#include <iostream>
using namespace std;

Enemy::Enemy(string name, int hp, int atk, int def,
             int exp, int gold, string type)
    : name(name), hp(hp), attack(atk), defense(def),
      expReward(exp), goldReward(gold), type(type) {}

bool Enemy::isAlive() const {
    return hp > 0;
}

int Enemy::takeDamage(int dmg) {
    int actual = dmg - defense;
    if (actual < 1) actual = 1;
    hp -= actual;
    if (hp < 0) hp = 0;
    return actual;
}

void Enemy::printStatus() const {
    cout << name << " [HP: " << hp << "]" << endl;
}
