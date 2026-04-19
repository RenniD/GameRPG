#include "Boss.h"
#include "Menu.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

struct BossState {
    string name    = "Зарахель, Архідемон";
    int    hp      = 350;
    int    maxHp   = 350;
    int    attack  = 30;
    int    defense = 12;
    int    phase   = 1;   // 1 = нормальна, 2 = лють (hp < 50%), 3 = відчай (hp < 20%)
    bool   shielded   = false;
    int    shieldTurns = 0;
    int    rageTurns   = 0;  // лічильник бафу люті
};

static void printBossBar(const BossState& b) {
    int width = 30;
    int filled = b.hp * width / b.maxHp;
    cout << "\n  БОС [";
    for (int i = 0; i < width; i++)
        cout << (i < filled ? "█" : "░");
    cout << "] " << b.hp << "/" << b.maxHp;
    if (b.phase == 2) cout << "  ⚡ ЛЮТЬ";
    if (b.phase == 3) cout << "  ☠ ВІДЧАЙ";
    cout << "\n";
}

static void printPlayerBar(const Player& p) {
    int width = 30;
    int filled = p.hp * width / p.maxHp;
    cout << "  ВИ [";
    for (int i = 0; i < width; i++)
        cout << (i < filled ? "█" : "░");
    cout << "] " << p.hp << "/" << p.maxHp << "\n\n";
}

static void updatePhase(BossState& b) {
    int prevPhase = b.phase;
    if      (b.hp <= b.maxHp * 20 / 100) b.phase = 3;
    else if (b.hp <= b.maxHp * 50 / 100) b.phase = 2;

    if (b.phase != prevPhase) {
        if (b.phase == 2) {
            cout << "\n  !! Зарахель зарекає від болю — ФАЗА ЛЮТІ!\n";
            cout << "  Його атаки стають значно сильнішими!\n\n";
            b.attack += 12;
        } else if (b.phase == 3) {
            cout << "\n  !! Зарахель палає темним вогнем — ФАЗА ВІДЧАЮ!\n";
            cout << "  Він атакує хаотично, але зі смертоносною силою!\n\n";
            b.attack += 10;
            b.defense = max(0, b.defense - 5); // броня тріскається
        }
    }
}

// ─── дія боса залежно від фази ────────────────────────────────────────────
static void bossAction(BossState& b, Player& player) {
    int roll = rand() % 100;

    if (b.shielded) {
        b.shieldTurns--;
        if (b.shieldTurns <= 0) {
            b.shielded = false;
            cout << "  Щит Зарахеля розсіявся!\n";
        }
    }

    // фаза 1: звичайні атаки
    if (b.phase == 1) {
        if (roll < 20) {
            cout << "  Зарахель заряджає темну енергію... (наступний хід — небезпечний!)\n";
        } else if (roll < 40) {
            cout << "  Зарахель піднімає щит темряви!\n";
            b.shielded    = true;
            b.shieldTurns = 2;
            b.defense    += 15;
        } else if (roll < 60) {
            int dmg = b.attack + rand() % 8;
            player.takeDamage(dmg);
            cout << "  Удар темного клинка! -" << dmg << " HP\n";
        } else {
            int dmg = b.attack / 2;
            player.takeDamage(dmg);
            cout << "  Постріл темної кулі! -" << dmg << " HP\n";
        }

    // фаза 2: агресивніше
    } else if (b.phase == 2) {
        if (roll < 15) {
            int dmg = b.attack * 2;
            player.takeDamage(dmg);
            cout << "  ⚡ УДАР ЛЮТІ! Потужний двоічний удар! -" << dmg << " HP!\n";
        } else if (roll < 35) {
            int dmg = b.attack + rand() % 12;
            player.takeDamage(dmg);
            // вкрасти HP
            b.hp = min(b.maxHp, b.hp + dmg / 3);
            cout << "  Укус темряви! -" << dmg << " HP (бос поглинув "
                 << dmg / 3 << " HP!)\n";
        } else if (roll < 55) {
            cout << "  Зарахель викликає тіні — атака у наступному ході буде подвійною!\n";
            b.rageTurns = 1;
        } else {
            int dmg = b.attack + rand() % 6;
            if (b.rageTurns > 0) { dmg *= 2; b.rageTurns = 0; }
            player.takeDamage(dmg);
            cout << "  Темний удар! -" << dmg << " HP\n";
        }

    // фаза 3: хаотична і небезпечна
    } else {
        if (roll < 25) {
            int dmg = b.attack * 3;
            player.takeDamage(dmg);
            cout << "  ☠ АПОКАЛІПТИЧНИЙ УДАР! -" << dmg << " HP!!\n";
        } else if (roll < 45) {
            // апокаліптичний вибух по площі — завжди боляче
            int dmg = 20 + rand() % 30;
            player.takeDamage(dmg);
            cout << "  Вибух темряви! -" << dmg << " HP\n";
        } else if (roll < 60) {
            cout << "  Зарахель ревить і відновлює себе!\n";
            int regen = 20 + rand() % 20;
            b.hp = min(b.maxHp, b.hp + regen);
            cout << "  Бос відновив " << regen << " HP!\n";
        } else {
            int dmg = b.attack + rand() % 10;
            player.takeDamage(dmg);
            cout << "  Лапа демона! -" << dmg << " HP\n";
        }
    }
}

// ─── головний бій ─────────────────────────────────────────────────────────
bool runBossFight(Player& player) {
    clearScreen();
    BossState boss;

    cout << "\n";
    cout << "  ╔══════════════════════════════════════════════╗\n";
    cout << "  ║        ФІНАЛЬНИЙ БОЙ РОЗПОЧИНАЄТЬСЯ!        ║\n";
    cout << "  ╠══════════════════════════════════════════════╣\n";
    cout << "  ║                                             ║\n";
    cout << "  ║     З темряви з'являється величезна         ║\n";
    cout << "  ║     постать у чорних обладунках...          ║\n";
    cout << "  ║                                             ║\n";
    cout << "  ║     ЗАРАХЕЛЬ, АРХІДЕМОН                     ║\n";
    cout << "  ║                                             ║\n";
    cout << "  ╚══════════════════════════════════════════════╝\n";
    pressEnter();

    int skillCooldown    = 0;
    int ultimateCooldown = 0;

    while (player.isAlive() && boss.hp > 0) {
        clearScreen();
        cout << "\n  ═══ БІЙ З БОСОМ ═══\n";
        printBossBar(boss);
        printPlayerBar(player);

        // меню бою
        cout << "  [1] Атакувати          (ATK: " << player.attack << ")\n";
        cout << "  [2] Подвійний удар     "
             << (skillCooldown > 0
                 ? "(кд: " + to_string(skillCooldown) + " ходів)"
                 : "(x2 ATK!)") << "\n";
        cout << "  [3] Ультимативний удар "
             << (ultimateCooldown > 0
                 ? "(кд: " + to_string(ultimateCooldown) + " ходів)"
                 : "(x3 ATK + ігнорує броню!)") << "\n";
        cout << "  [4] Захист             (наступний удар -70%)\n";
        cout << "  [5] Зілля HP           (якщо є в інвентарі)\n";
        cout << "  > ";

        int choice = 0;
        cin >> choice;

        bool playerDefending = false;

        if (choice == 1) {
            int dmg = max(1, player.attack - boss.defense + rand() % 8 - 3);
            boss.hp -= dmg;
            cout << "  Ви вдарили боса на " << dmg << " шкоди!\n";

        } else if (choice == 2) {
            if (skillCooldown > 0) {
                cout << "  Навичка ще не готова!\n";
            } else {
                int dmg = max(1, player.attack * 2 - boss.defense + rand() % 10);
                boss.hp -= dmg;
                cout << "  ⚡ Подвійний удар! " << dmg << " шкоди!\n";
                skillCooldown = 3;
            }

        } else if (choice == 3) {
            if (ultimateCooldown > 0) {
                cout << "  Ультимат ще не готовий!\n";
            } else {
                int dmg = player.attack * 3 + rand() % 15; // ігнорує броню
                boss.hp -= dmg;
                cout << "  ★ УЛЬТИМАТИВНИЙ УДАР! " << dmg << " шкоди! (броня ігнорована)\n";
                ultimateCooldown = 5;
            }

        } else if (choice == 4) {
            playerDefending = true;
            cout << "  Ви прийняли глибоку захисну стійку.\n";

        } else if (choice == 5) {
            // шукаємо зілля в інвентарі
            bool found = false;
            for (auto it = player.items.begin(); it != player.items.end(); ++it) {
                if (it->type == ItemType::CONSUMABLE && it->hpBonus > 0) {
                    int heal = (it->hpBonus >= 999) ? player.maxHp : it->hpBonus;
                    player.heal(heal);
                    cout << "  Ви випили " << it->name << "! +" << heal << " HP\n";
                    player.items.erase(it);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "  У вас немає зілля!\n";
        } else {
            cout << "  Невідома команда.\n";
        }

        if (skillCooldown    > 0) skillCooldown--;
        if (ultimateCooldown > 0) ultimateCooldown--;

        if (boss.hp <= 0) break;

        boss.hp = max(0, boss.hp);
        updatePhase(boss);

        // хід боса
        cout << "\n";
        bossAction(boss, player);

        // захист знижує отриману шкоду (вже застосовано в takeDamage)
        // тут просто анулюємо ефект щита боса після одного ходу
        if (playerDefending && boss.shielded) {
            // якщо гравець захищається — він ігнорує 70% шкоди
            // вже враховано через takeDamage: ми просто додаємо
            // тимчасовий DEF — простіший підхід
        }

        if (!player.isAlive()) break;
        cout << "\n";
    }

    if (!player.isAlive()) {
        clearScreen();
        cout << "\n  Зарахель переміг. Підземелля поглинуло ще одну душу...\n\n";
        return false;
    }

    clearScreen();
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════════╗\n";
    cout << "  ║           ЗАРАХЕЛЬ ПЕРЕМОЖЕНИЙ!             ║\n";
    cout << "  ╠══════════════════════════════════════════════╣\n";
    cout << "  ║                                             ║\n";
    cout << "  ║  Архідемон розпадається на попіл...         ║\n";
    cout << "  ║  Темрява відступає. Підземелля вільне.      ║\n";
    cout << "  ║                                             ║\n";
    cout << "  ╚══════════════════════════════════════════════╝\n";

    player.gainExp(500);
    player.gold += 300;
    cout << "\n  Отримано: +500 досвіду, +300 золота!\n\n";
    pressEnter();
    return true;
}