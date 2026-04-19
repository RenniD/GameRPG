#pragma once
#include "Player.h"
#include "Item.h"
#include <vector>
using namespace std;

class Shop {
public:
    vector<Item> stock;

    Shop();
    void restock(int level);       // заповнити полиці під рівень
    void open(Player& player);     // інтерактивний магазин
    void sell(Player& player);     // продаж предметів гравця

private:
    void printStock()              const;
    void printPlayerItems(const Player& p) const;
};