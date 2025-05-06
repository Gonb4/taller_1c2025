#include "stdio_manager.h"

#include <iostream>
#include <sstream>

namespace {
const char* CreateGameCmd = "crear";
const char* JoinGameCmd = "unirse";
const char* ListGamesCmd = "listar";

const char* GameMoveCmd = "jugar";

constexpr char QuitChar = 'q';
}  // namespace


StdIOManager::StdIOManager() {}


// ======================= CLIENT =======================

Operation StdIOManager::read_operation() {
    std::string line;
    std::string command;

    while (true) {
        std::getline(std::cin, line);
        std::istringstream iss(line);
        iss >> command;

        if (command == CreateGameCmd) {
            std::string game_name;
            if (iss >> game_name)
                return CreateGameOp(game_name);
        } else if (command == JoinGameCmd) {
            std::string game_name;
            if (iss >> game_name)
                return JoinGameOp(game_name);
        } else if (command == ListGamesCmd) {
            return ListGamesOp();
        }
    }
}

PlayerMove StdIOManager::read_game_move() {
    std::string line;
    std::string command;

    while (true) {
        std::getline(std::cin, line);
        std::istringstream iss(line);
        iss >> command;

        if (command == GameMoveCmd) {
            int col;
            int row;
            if (iss >> col >> row)
                if (1 <= col && col <= 3 && 1 <= row && row <= 3)
                    return PlayerMove(col - 1, row - 1);
        }
    }
}

void StdIOManager::print(const std::string& msg) { std::cout << msg; }

// ======================= SERVER =======================

void StdIOManager::read_quit() {
    int c;
    while ((c = std::cin.get()) != QuitChar) {
        if (c == EOF)
            break;
    }
}
