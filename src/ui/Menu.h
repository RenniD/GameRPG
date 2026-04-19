#pragma once
#include <string>
using namespace std;

void showSplash();
void showHelp();
int  showMainMenu();          // 1=нова гра, 2=завантажити, 3=довідка, 4=вийти
string showStartWindow();
void showLevelTransition(int nextLevel);
void showShopTransition(int level);
void showVictory(const class Player& p);
void showGameOver(const class Player& p);
void clearScreen();
void pressEnter();