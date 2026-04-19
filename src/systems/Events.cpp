#include "Events.h"
#include "Combat.h"
#include "Item.h"
#include <iostream>
#include <cstdlib>
using namespace std;

static void handleTrap(Player& player, Room& room) {
    if (room.cleared) { cout << "  Пастка вже спрацювала.\n"; return; }
    cout << "  ⚠ ПАСТКА! (" << room.loot << ")\n";
    cout << "  " << room.description << "\n\n";
    cout << "  [1] Спробувати обійти (50% успіху)\n";
    cout << "  [2] Пробігти навпростець\n  > ";
    int ch; cin >> ch;
    if (ch == 1) {
        if (rand() % 2 == 0) cout << "  Уникнули!\n";
        else { player.takeDamage(room.trapDmg / 2); cout << "  Частково! -" << room.trapDmg/2 << " HP\n"; }
    } else {
        player.takeDamage(room.trapDmg);
        cout << "  Боляче! -" << room.trapDmg << " HP\n";
    }
    room.cleared = true;
}

static void handleChest(Player& player, Room& room) {
    if (room.cleared) { cout << "  Сундук порожній.\n"; return; }
    cout << "  " << room.description << "\n\n";
    cout << "  [1] Відкрити  [2] Залишити\n  > ";
    int ch; cin >> ch;
    if (ch != 1) { cout << "  Ви пройшли мимо.\n"; return; }

    if (rand() % 10 == 0) {
        int curse = 15 + rand() % 20;
        player.takeDamage(curse);
        cout << "  Проклятий сундук! -" << curse << " HP!\n";
        room.cleared = true; return;
    }

    // предмети у скрині
    static vector<Item> chestItems = {
        Item("Зілля здоров'я",  "Відновлює 30 HP",  ItemType::CONSUMABLE, 20, 0, 30),
        Item("Зілля сили",      "ATK +5 назавжди",  ItemType::CONSUMABLE, 50, 5,  0),
        Item("Залізний меч",    "ATK +8",            ItemType::WEAPON,     60, 8,  0),
        Item("Шкіряна броня",   "DEF +5",            ItemType::ARMOR,      50, 5,  0),
        Item("Кільце регенерації","DEF +4, HP +20",  ItemType::ACCESSORY,  90, 4, 20),
    };
    Item found = chestItems[rand() % chestItems.size()];
    cout << "  Ви знайшли: " << found.name << "!\n";
    cout << "  " << found.description << "\n";

    // одразу застосовуємо ефект або кладемо в інвентар
    if (found.type == ItemType::CONSUMABLE) {
        if (found.hpBonus > 0)   { player.heal(found.hpBonus); cout << "  +" << found.hpBonus << " HP\n"; }
        if (found.statBonus > 0) { player.attack += found.statBonus; cout << "  +ATK " << found.statBonus << "\n"; }
    } else {
        if (found.type == ItemType::WEAPON)    player.attack  += found.statBonus;
        if (found.type == ItemType::ARMOR)     player.defense += found.statBonus;
        if (found.type == ItemType::ACCESSORY) {
            player.attack  += found.statBonus;
            player.defense += found.statBonus;
            if (found.hpBonus) { player.maxHp += found.hpBonus; player.hp += found.hpBonus; }
        }
        player.addItem(found);
        cout << "  Предмет додано в інвентар.\n";
    }
    room.cleared = true;
}

static void handleFountain(Player& player, Room& room) {
    if (room.cleared) { cout << "  Фонтан висох.\n"; return; }
    cout << "  Невідомий фонтан мерехтить у темряві.\n\n";
    cout << "  [1] Випити  [2] Вмити обличчя  [3] Пройти мимо\n  > ";
    int ch; cin >> ch;
    if (ch == 3) { cout << "  Ви пройшли мимо.\n"; return; }
    if (ch == 2) { cout << "  Прохолодна вода освіжає. Нічого особливого.\n"; room.cleared = true; return; }

    int roll = rand() % 4;
    if (roll == 0) {
        int h = 40 + rand() % 31; player.heal(h);
        cout << "  Магічна вода! +" << h << " HP\n";
    } else if (roll == 1) {
        player.attack += 3;
        cout << "  Вода сили! ATK +3\n";
    } else if (roll == 2) {
        int d = 15 + rand() % 15; player.takeDamage(d);
        cout << "  Отруйна вода! -" << d << " HP!\n";
    } else {
        player.maxHp += 20; player.hp += 20;
        cout << "  Blessed вода! MaxHP +20\n";
    }
    room.cleared = true;
}

static void handleShrine(Player& player, Room& room) {
    if (room.cleared) { cout << "  Святилище вже використане.\n"; return; }
    cout << "  Стародавнє святилище пульсує синім вогнем.\n\n";
    cout << "  [1] Помолитися (відновити 50% HP)\n";
    cout << "  [2] Пожертвувати 50 золота (ATK +10)\n";
    cout << "  [3] Піти геть\n  > ";
    int ch; cin >> ch;
    if (ch == 1) {
        int h = player.maxHp / 2; player.heal(h);
        cout << "  Синє сяйво! +" << h << " HP\n"; room.cleared = true;
    } else if (ch == 2) {
        if (player.gold >= 50) {
            player.gold -= 50; player.attack += 10;
            cout << "  Богиня прийняла! ATK +10\n"; room.cleared = true;
        } else cout << "  Недостатньо золота (потрібно 50).\n";
    } else {
        cout << "  Ви вирішили не тривожити стародавні сили.\n";
    }
}

bool handleRoom(Player& player, Room& room) {
    cout << "\n  ── " << room.description << "\n\n";
    switch (room.type) {
        case RoomType::EMPTY:   cout << "  Тут нічого особливого.\n"; break;
        case RoomType::ENEMY:
            if (room.cleared) { cout << "  Тут вже нічого немає.\n"; break; }
            if (room.enemy && room.enemy->isAlive()) {
                bool won = runCombat(player, *room.enemy);
                if (!player.isAlive()) return false;
                if (won) room.cleared = true;
            }
            break;
        case RoomType::TRAP:     handleTrap(player, room);    if (!player.isAlive()) return false; break;
        case RoomType::CHEST:    handleChest(player, room);   break;
        case RoomType::FOUNTAIN: handleFountain(player, room);if (!player.isAlive()) return false; break;
        case RoomType::SHRINE:   handleShrine(player, room);  break;
        case RoomType::EXIT:     cout << "  Ви бачите сходи вгору!\n"; break;
        default: break;
    }
    return true;
}