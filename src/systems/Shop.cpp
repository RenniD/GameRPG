#include "Shop.h"
#include "Menu.h"
#include <iostream>
#include <limits>
using namespace std;

// ─── каталог усіх можливих товарів ───────────────────────────────────────
static vector<Item> allItems() {
    return {
        // Зілля
        Item("Мале зілля HP",    "Відновлює 30 HP",              ItemType::CONSUMABLE, 20,  0,  30),
        Item("Велике зілля HP",  "Відновлює 60 HP",              ItemType::CONSUMABLE, 40,  0,  60),
        Item("Зілля сили",       "ATK +5 назавжди",              ItemType::CONSUMABLE, 50,  5,   0),
        Item("Зілля витривалості","DEF +3 назавжди",             ItemType::CONSUMABLE, 45,  3,   0),
        Item("Еліксир",          "Відновлює всі HP",             ItemType::CONSUMABLE, 80,  0, 999),
        // Зброя
        Item("Залізний меч",     "ATK +8",                       ItemType::WEAPON,     60,  8,   0),
        Item("Срібний клинок",   "ATK +14",                      ItemType::WEAPON,    100, 14,   0),
        Item("Руничний топір",   "ATK +20",                      ItemType::WEAPON,    160, 20,   0),
        Item("Темний клинок",    "ATK +28",                      ItemType::WEAPON,    240, 28,   0),
        // Броня
        Item("Шкіряна броня",    "DEF +5",                       ItemType::ARMOR,      50,  5,   0),
        Item("Кольчуга",         "DEF +10",                      ItemType::ARMOR,     100, 10,   0),
        Item("Лицарські лати",   "DEF +16",                      ItemType::ARMOR,     170, 16,   0),
        // Аксесуари
        Item("Кільце регенерації","DEF +4, HP +20",              ItemType::ACCESSORY,  90,  4,  20),
        Item("Амулет мудреця",   "ATK +6, DEF +6",               ItemType::ACCESSORY, 130,  6,   0),
        Item("Корона безстрашшя","ATK +10, MaxHP +30",           ItemType::ACCESSORY, 200, 10,  30),
    };
}

Shop::Shop() { restock(1); }

void Shop::restock(int level) {
    stock.clear();
    auto all = allItems();

    // рівень 1: тільки дешеві предмети (value <= 60)
    // рівень 2: до 130
    // рівень 3: все
    int maxVal = (level == 1) ? 60 : (level == 2) ? 130 : 9999;

    for (auto& item : all)
        if (item.value <= maxVal)
            stock.push_back(item);
}

void Shop::printStock() const {
    cout << "\n  ┌─────────────────────────────────────────────────────┐\n";
    cout << "  │  №   Тип          Назва                  Ціна      │\n";
    cout << "  ├─────────────────────────────────────────────────────┤\n";
    for (size_t i = 0; i < stock.size(); i++) {
        const auto& it = stock[i];
        cout << "  │  [" << (i + 1) << "]  "
             << it.typeLabel() << "  ";
        // вирівнювання назви
        string nm = it.name;
        while (nm.size() < 22) nm += ' ';
        cout << nm << "  " << it.value << " зол.";
        // доповнення до краю
        cout << "\n";
        cout << "  │        " << it.description << "\n";
    }
    cout << "  └─────────────────────────────────────────────────────┘\n";
}

void Shop::printPlayerItems(const Player& p) const {
    cout << "\n  Ваші предмети (для продажу):\n";
    if (p.items.empty()) {
        cout << "  (порожньо)\n";
        return;
    }
    for (size_t i = 0; i < p.items.size(); i++) {
        cout << "  [" << (i + 1) << "] "
             << p.items[i].typeLabel() << " "
             << p.items[i].name
             << "  (ціна продажу: " << p.items[i].value / 2 << " зол.)\n";
    }
}

void Shop::open(Player& player) {
    while (true) {
        clearScreen();
        cout << "\n  ╔══════════════════════════════════════╗\n";
        cout << "  ║          КРАМНИЦЯ МАНДРІВНИКА        ║\n";
        cout << "  ╚══════════════════════════════════════╝\n";
        cout << "  Золото: " << player.gold << "\n";

        printStock();

        cout << "\n  [1-" << stock.size() << "] Купити"
             << "  [S] Продати предмети"
             << "  [Q] Вийти з крамниці\n  > ";

        string input;
        cin >> input;

        if (input == "Q" || input == "q") break;

        if (input == "S" || input == "s") {
            sell(player);
            continue;
        }

        // спроба зчитати число
        try {
            int idx = stoi(input) - 1;
            if (idx < 0 || idx >= (int)stock.size()) {
                cout << "  Невірний номер.\n";
                pressEnter();
                continue;
            }
            Item& chosen = stock[idx];
            if (player.gold < chosen.value) {
                cout << "  Недостатньо золота!\n";
                pressEnter();
                continue;
            }

            player.gold -= chosen.value;
            cout << "  Ви купили: " << chosen.name << "!\n";

            // застосовуємо ефект одразу або кладемо в інвентар
            if (chosen.type == ItemType::CONSUMABLE) {
                if (chosen.hpBonus >= 999) {
                    player.hp = player.maxHp;
                    cout << "  HP повністю відновлено!\n";
                } else if (chosen.hpBonus > 0) {
                    player.heal(chosen.hpBonus);
                    cout << "  Відновлено " << chosen.hpBonus << " HP.\n";
                }
                if (chosen.statBonus > 0) {
                    if (chosen.name.find("сили") != string::npos) {
                        player.attack += chosen.statBonus;
                        cout << "  ATK +" << chosen.statBonus << "!\n";
                    } else {
                        player.defense += chosen.statBonus;
                        cout << "  DEF +" << chosen.statBonus << "!\n";
                    }
                }
            } else {
                // зброя, броня, аксесуар — кладемо в інвентар і застосовуємо
                player.attack  += (chosen.type == ItemType::WEAPON)  ? chosen.statBonus : 0;
                player.defense += (chosen.type == ItemType::ARMOR)   ? chosen.statBonus : 0;
                if (chosen.type == ItemType::ACCESSORY) {
                    player.attack  += chosen.statBonus;
                    player.defense += chosen.statBonus;
                    if (chosen.hpBonus > 0) {
                        player.maxHp += chosen.hpBonus;
                        player.hp    += chosen.hpBonus;
                    }
                }
                player.items.push_back(chosen);
                cout << "  Предмет додано в інвентар.\n";
            }

            pressEnter();

        } catch (...) {
            cout << "  Невідома команда.\n";
            pressEnter();
        }
    }
}

void Shop::sell(Player& player) {
    while (true) {
        clearScreen();
        cout << "\n  ── Продаж предметів ──\n";
        cout << "  Золото: " << player.gold << "\n";
        printPlayerItems(player);

        if (player.items.empty()) { pressEnter(); return; }

        cout << "\n  [номер] Продати  [Q] Назад\n  > ";
        string input; cin >> input;
        if (input == "Q" || input == "q") return;

        try {
            int idx = stoi(input) - 1;
            if (idx < 0 || idx >= (int)player.items.size()) {
                cout << "  Невірний номер.\n"; pressEnter(); continue;
            }
            Item& it = player.items[idx];
            int price = it.value / 2;

            // знімаємо бонуси
            if (it.type == ItemType::WEAPON)    player.attack  -= it.statBonus;
            if (it.type == ItemType::ARMOR)     player.defense -= it.statBonus;
            if (it.type == ItemType::ACCESSORY) {
                player.attack  -= it.statBonus;
                player.defense -= it.statBonus;
                if (it.hpBonus > 0) {
                    player.maxHp -= it.hpBonus;
                    player.hp     = min(player.hp, player.maxHp);
                }
            }
            player.gold += price;
            cout << "  Продано: " << it.name << " за " << price << " золота.\n";
            player.items.erase(player.items.begin() + idx);
            pressEnter();

        } catch (...) {
            cout << "  Невідома команда.\n"; pressEnter();
        }
    }
}