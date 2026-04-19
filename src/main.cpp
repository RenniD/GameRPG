#include "Player.h"
#include "Maze.h"
#include "Events.h"
#include "Menu.h"
#include "Shop.h"
#include "Boss.h"
#include <iostream>
#include <limits>
using namespace std;

// ─── один рівень ─────────────────────────────────────────────────────────
bool runLevel(Player& player, Maze& maze, int level) {
    maze.generate(level);
    player.x = 0;
    player.y = 0;

    while (true) {
        clearScreen();
        cout << "\n  ═══ РІВЕНЬ " << level
             << "  |  (" << player.x << "," << player.y << ") ═══\n";
        player.printStatus();
        maze.display(player);

        Room& cur = maze.grid[player.x][player.y];
        cur.visited = true;

        if (!handleRoom(player, cur)) return false;
        if (cur.type == RoomType::EXIT) {
            cout << "\n  Ви знайшли вихід з рівня " << level << "!\n";
            // автозбереження при виході з рівня
            pressEnter();
            return true;
        }

        cout << "\n  Куди? (w/a/s/d) | [i] інвентар | [q] пауза\n  > ";
        char cmd; cin >> cmd;

        if (cmd == 'i') { player.printInventory(); pressEnter(); continue; }

        if (cmd == 'q') {
            cout << "  [1] Продовжити  [2] Зберегти і вийти в меню\n  > ";
            int qch; cin >> qch;
            if (qch == 2) { return false; }
            continue;
        }

        int nx = player.x, ny = player.y;
        if      (cmd == 'w') ny--;
        else if (cmd == 's') ny++;
        else if (cmd == 'a') nx--;
        else if (cmd == 'd') nx++;
        else { cout << "  Невідома команда.\n"; pressEnter(); continue; }

        if (nx < 0 || nx >= MAZE_SIZE || ny < 0 || ny >= MAZE_SIZE) {
            cout << "  Стіна!\n"; pressEnter(); continue;
        }
        player.x = nx; player.y = ny;
        maze.grid[player.x][player.y].visited = true;
    }
}

// ─── main ────────────────────────────────────────────────────────────────
int main() {
    showSplash();
    Shop shop;

    while (true) {
        int menuChoice = showMainMenu();

        if (menuChoice == 4) { clearScreen(); cout << "\n  До побачення!\n\n"; break; }
        if (menuChoice == 3) { showHelp(); continue; }

        Player player("Герой");
        int    startLevel = 1;
        Maze   maze;

        if (menuChoice == 2) {
            // завантаження збереження
        } else {
            // нова гра
            string name = showStartWindow();
            player = Player(name);
        }

        bool alive = true;

        for (int level = startLevel; level <= 3 && alive; level++) {
            if (level > startLevel || menuChoice == 1) {
                if (level > 1) showLevelTransition(level);
            }

            alive = runLevel(player, maze, level);
            if (!alive) break;

            // ─── крамниця між рівнями (після 1 і 2) ───────────
            if (level < 3) {
                showShopTransition(level);
                shop.restock(level);
                shop.open(player);
            }
        }

        // ─── фінальний бос після рівня 3 ─────────────────────
        if (alive) {
            clearScreen();
            cout << "\n  Ви пройшли всі три рівні.\n";
            cout << "  Але попереду — остання кімната...\n";
            pressEnter();
            alive = runBossFight(player);
        }

        if (alive) {
            showVictory(player);
        } else if (!player.isAlive()) {
            showGameOver(player);
        }

        pressEnter();
    }

    return 0;
}