#pragma once
#include "Room.h"
#include "Player.h"
#include <vector>
using namespace std;

const int MAZE_SIZE = 5;

class Maze {
public:
    vector<vector<Room>> grid;
    int currentLevel;

    Maze();
    void generate(int level);
    void display(const Player& p) const;

private:
    Enemy* makeEnemy(int level);
};