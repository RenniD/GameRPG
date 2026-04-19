#include "Combat.h"
#include <iostream>
#include <cstdlib>
using namespace std;

static void printBar(const string& label, int cur, int max, int width = 20) {
    int filled = (max > 0) ? (cur * width / max) : 0;
    cout << "  " << label << " [";
    for (int i = 0; i < width; i++)
        cout << (i < filled ? "█" : "░");
    cout << "] " << cur << "/" << max << "\n";
}

bool runCombat(Player& player, Enemy& enemy) {
    cout << "\n  ╔══════════════════════════════════╗\n";
    cout << "  ║         ПОЧИНАЄТЬСЯ БІЙ!         ║\n";
    cout << "  ╚══════════════════════════════════╝\n";
    cout << "  Противник: " << enemy.name;
    if (enemy.type == "boss") cout << "  ⚠ [БОС]";
    if (enemy.type == "elite") cout << "  ★ [ЕЛІТА]";
    cout << "\n\n";

    int playerSkillCooldown = 0;

    while (player.isAlive() && enemy.isAlive()) {
        // ─── статус ───────────────────────────────────────
        printBar("ВАШ  HP ", player.hp, player.maxHp);
        printBar("ВОРОГ HP", enemy.hp,  enemy.hp > 0 ? enemy.hp + 1 : 1);
        // (просто щоб бар не був завжди повним — показуємо поточне)
        // Переробимо: зберігаємо maxHp ворога окремо (через static trick)
        cout << "\n";

        // ─── меню дій ────────────────────────────────────
        cout << "  [1] Атакувати  (ATK: " << player.attack << ")\n";
        cout << "  [2] Навичка    "
             << (playerSkillCooldown > 0
                 ? "(кд: " + to_string(playerSkillCooldown) + " ходів)"
                 : "(подвійний удар!")
             << "\n";
        cout << "  [3] Захист     (наступний удар -50% шкоди)\n";
        cout << "  [4] Спробувати втекти\n";
        cout << "  > ";

        int choice = 0;
        cin >> choice;

        bool playerDefending = false;

        if (choice == 1) {
            // звичайна атака
            int dmg = player.attack + rand() % 6 - 2; // ±2 випадковість
            int dealt = enemy.takeDamage(dmg);
            cout << "  Ви вдарили " << enemy.name << " на " << dealt << " шкоди!\n";

        } else if (choice == 2) {
            if (playerSkillCooldown > 0) {
                cout << "  Навичка ще не готова!\n";
            } else {
                int dmg = player.attack * 2 + rand() % 8;
                int dealt = enemy.takeDamage(dmg);
                cout << "  ⚡ ПОДВІЙНИЙ УДАР! " << dealt << " шкоди!\n";
                playerSkillCooldown = 3;
            }

        } else if (choice == 3) {
            playerDefending = true;
            cout << "  Ви прийняли захисну стійку.\n";

        } else if (choice == 4) {
            int escapeRoll = rand() % 100;
            int escapeChance = 40 - (enemy.type == "boss" ? 30 : 0);
            if (escapeRoll < escapeChance) {
                cout << "  Вам вдалося втекти!\n\n";
                return false;
            } else {
                cout << "  Шлях відрізаний! Втеча не вдалася!\n";
            }

        } else {
            cout << "  Невідома команда. Ви пропустили хід.\n";
        }

        if (playerSkillCooldown > 0) playerSkillCooldown--;

        // ─── хід ворога ──────────────────────────────────
        if (enemy.isAlive()) {
            int atkRoll = rand() % 100;

            if (atkRoll < 15 && enemy.type != "weak") {
                // спеціальна атака
                int dmg = enemy.attack * 2;
                if (!playerDefending) player.takeDamage(dmg);
                else player.takeDamage(dmg / 4);
                cout << "  !! " << enemy.name << " використав особливу атаку! ("
                     << (playerDefending ? dmg / 4 : dmg) << " шкоди)\n";

            } else if (atkRoll < 25) {
                cout << "  " << enemy.name << " промахнувся!\n";

            } else {
                int atkDmg = enemy.attack + rand() % 5 - 2;
                if (playerDefending) atkDmg /= 2;
                player.takeDamage(atkDmg);
                cout << "  " << enemy.name << " атакував вас ("
                     << atkDmg << " шкоди)"
                     << (playerDefending ? " [захист]" : "") << "\n";
            }
        }

        cout << "\n";
    }

    // ─── результат ───────────────────────────────────────
    if (!player.isAlive()) {
        cout << "  Ви загинули у бою з " << enemy.name << "...\n\n";
        return false;
    }

    cout << "  ★ " << enemy.name << " переможений!\n";
    player.gainExp(enemy.expReward);
    player.gold += enemy.goldReward;
    cout << "  Отримано: +" << enemy.expReward << " досвіду, +"
         << enemy.goldReward << " золота\n\n";
    return true;
}