#include "Maze.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ─── фабрика ворогів залежно від рівня ─────────────────────────────────────
Enemy* Maze::makeEnemy(int lvl) {
    int r = rand() % 100;

    // Рівень 1
    if (lvl == 1) {
        if (r < 40)
            return new Enemy("Гоблін",      30,  8, 1, 25, 10, "weak");
        if (r < 70)
            return new Enemy("Скелет",      45, 12, 3, 40, 20, "normal");
        if (r < 90)
            return new Enemy("Орк",         60, 15, 5, 55, 30, "normal");
        return     new Enemy("Темний лицар",90, 20, 8,100, 60, "elite");
    }
    // Рівень 2
    if (lvl == 2) {
        if (r < 35)
            return new Enemy("Зомбі",       50, 12, 2, 35, 15, "weak");
        if (r < 65)
            return new Enemy("Тролль",      80, 18, 6, 60, 35, "normal");
        if (r < 85)
            return new Enemy("Вампір",     100, 22, 8, 80, 50, "elite");
        return     new Enemy("Дракончик", 130, 28,10,120, 80, "elite");
    }
    // Рівень 3+
    if (r < 30)
        return new Enemy("Демон",         80, 18, 5, 50, 30, "normal");
    if (r < 60)
        return new Enemy("Ліч",          120, 25, 8, 90, 60, "elite");
    if (r < 85)
        return new Enemy("Чорний дракон",160, 32,12,130, 90, "elite");
    return     new Enemy("Повелитель темряви",200,38,15,200,150,"boss");
}

// ─── конструктор ───────────────────────────────────────────────────────────
Maze::Maze() : currentLevel(1) {
    srand(static_cast<unsigned>(time(nullptr)));
}

// ─── генерація поля ────────────────────────────────────────────────────────
void Maze::generate(int level) {
    currentLevel = level;

    // перестворюємо сітку
    grid.clear();
    grid.resize(MAZE_SIZE);
    for (auto& col : grid) { col.clear(); for (int i = 0; i < MAZE_SIZE; i++) col.push_back(Room()); }

    // стартова клітинка — завжди порожня
    grid[0][0].type        = RoomType::EMPTY;
    grid[0][0].description = "Вхід до підземелля. Тут починається ваша пригода.";
    grid[0][0].visited     = true;

    // вихід — завжди у правому нижньому куті
    grid[MAZE_SIZE-1][MAZE_SIZE-1].type        = RoomType::EXIT;
    grid[MAZE_SIZE-1][MAZE_SIZE-1].description = "Сходи вгору. Звідси можна піти далі.";

    // заповнення решти клітинок
    for (int y = 0; y < MAZE_SIZE; y++) {
        for (int x = 0; x < MAZE_SIZE; x++) {
            if ((x == 0 && y == 0) ||
                (x == MAZE_SIZE-1 && y == MAZE_SIZE-1)) continue;

            int roll = rand() % 100;

            if (roll < 35) {
                // ворог — частіше на вищих рівнях
                grid[x][y].type  = RoomType::ENEMY;
                grid[x][y].enemy = makeEnemy(level);
                grid[x][y].description = "Тут хтось є...";

            } else if (roll < 50) {
                // пастка
                grid[x][y].type    = RoomType::TRAP;
                grid[x][y].trapDmg = 10 + rand() % (10 * level);
                int tr = rand() % 3;
                if (tr == 0) {
                    grid[x][y].description = "Підлога вкрита підозрілими плитами.";
                    grid[x][y].loot        = "шипи";
                } else if (tr == 1) {
                    grid[x][y].description = "Запах сірки відчувається у повітрі.";
                    grid[x][y].loot        = "вогняна пастка";
                } else {
                    grid[x][y].description = "Тут занадто тихо і темно.";
                    grid[x][y].loot        = "темна пастка";
                }

            } else if (roll < 62) {
                // скриня
                grid[x][y].type = RoomType::CHEST;
                int ch = rand() % 4;
                if (ch == 0) {
                    grid[x][y].description = "Загадковий скрипучий сундук стоїть у кутку.";
                    grid[x][y].loot        = "Зілля здоров'я";
                } else if (ch == 1) {
                    grid[x][y].description = "Невеликий дерев'яний скрині із залізним замком.";
                    grid[x][y].loot        = "Зілля сили";
                } else if (ch == 2) {
                    grid[x][y].description = "Стара скриня, вкрита рунами.";
                    grid[x][y].loot        = "Амулет захисту";
                } else {
                    grid[x][y].description = "Масивний золотий сундук!";
                    grid[x][y].loot        = "Мішок золота";
                }

            } else if (roll < 73) {
                // фонтан
                grid[x][y].type        = RoomType::FOUNTAIN;
                grid[x][y].description = "Невідомий фонтан виблискує у темряві.";

            } else if (roll < 82) {
                // святилище
                grid[x][y].type        = RoomType::SHRINE;
                grid[x][y].description = "Стародавнє святилище освітлене синім вогнем.";

            } else {
                // порожня
                grid[x][y].type        = RoomType::EMPTY;
                int ed = rand() % 3;
                if (ed == 0)
                    grid[x][y].description = "Покинута кімната. Лише пил і тиша.";
                else if (ed == 1)
                    grid[x][y].description = "Вузький коридор із щербатими стінами.";
                else
                    grid[x][y].description = "Тут раніше хтось жив — розбитий меблі скрізь.";
            }
        }
    }
}

// ─── відображення мапи ────────────────────────────────────────────────────
void Maze::display(const Player& p) const {
    cout << "\n  ╔";
    for (int x = 0; x < MAZE_SIZE; x++) cout << "════";
    cout << "╗\n";

    for (int y = 0; y < MAZE_SIZE; y++) {
        cout << "  ║";
        for (int x = 0; x < MAZE_SIZE; x++) {
            bool isPlayer = (p.x == x && p.y == y);
            if (isPlayer) { cout << " @ "; continue; }
            if (!grid[x][y].visited) { cout << " ? "; continue; }

            switch (grid[x][y].type) {
                case RoomType::EMPTY:    cout << " . "; break;
                case RoomType::ENEMY:
                    cout << (grid[x][y].cleared ? " . " : " E "); break;
                case RoomType::TRAP:
                    cout << (grid[x][y].cleared ? " . " : " ^ "); break;
                case RoomType::CHEST:
                    cout << (grid[x][y].cleared ? " . " : " $ "); break;
                case RoomType::FOUNTAIN:
                    cout << (grid[x][y].cleared ? " . " : " ~ "); break;
                case RoomType::SHRINE:
                    cout << (grid[x][y].cleared ? " . " : " * "); break;
                case RoomType::EXIT:     cout << " X "; break;
                default:                 cout << "   "; break;
            }
        }
        cout << "║\n";
    }

    cout << "  ╚";
    for (int x = 0; x < MAZE_SIZE; x++) cout << "════";
    cout << "╝\n";

    cout << "  Легенда: @=Ви  E=Ворог  ^=Пастка  $=Скриня"
            "  ~=Фонтан  *=Святилище  X=Вихід  ?=Невідомо\n\n";
}