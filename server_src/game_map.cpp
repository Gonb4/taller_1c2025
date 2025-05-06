#include "game_map.h"

#include <stdexcept>

GameMap::GameMap() {}


bool GameMap::create_game(const std::string& name) {
    std::unique_lock<std::mutex> lck(mtx);

    if (games.count(name))
        return false;

    games[name]; // Tateti solo tiene default constructor
    player_count[name] = 1;

    return true;
}

bool GameMap::join_game(const std::string& name) {
    std::unique_lock<std::mutex> lck(mtx);

    if (not games.count(name))
        return false;
    if (player_count[name] == 2)
        return false;

    player_count[name]++;

    return true;
}

std::vector<std::string> GameMap::list_games() {
    std::unique_lock<std::mutex> lck(mtx);

    std::vector<std::string> list;
    for (const auto& [k, v] : games) {
        list.push_back(k);
    }

    return list;
}

void GameMap::leave_game(const std::string& name) {
    std::unique_lock<std::mutex> lck(mtx);

    if (not games.count(name))
        throw std::runtime_error("Game not found");

    if (--player_count[name] == 0) {
        games.erase(name);
        player_count.erase(name);
    }
}

Tateti& GameMap::get_game(const std::string& name) {
    std::unique_lock<std::mutex> lck(mtx);

    if (not games.count(name))
        throw std::runtime_error("Game not found");

    return games[name];
}
