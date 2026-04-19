cmake_minimum_required(VERSION 4.1)
project(GameRPG)

set(CMAKE_CXX_STANDARD 20)

add_executable(GameRPG
        src/main.cpp
        src/entities/Player.cpp
        src/entities/Enemy.cpp
        src/entities/Boss.cpp
        src/world/Room.cpp
        src/world/Maze.cpp
        src/systems/Combat.cpp
        src/systems/Events.cpp
        src/systems/Shop.cpp
        src/ui/Menu.cpp
        src/items/Item.cpp
)

target_include_directories(GameRPG PRIVATE
        src/entities
        src/world
        src/systems
        src/ui
        src/items
)
