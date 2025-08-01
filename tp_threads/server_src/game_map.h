#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "tateti.h"

class GameMap {
private:
    std::map<std::string, Tateti> games;
    std::map<std::string, int> player_count;
    std::mutex mtx;

public:
    GameMap();

    bool create_game(const std::string& name);
    bool join_game(const std::string& name);
    std::vector<std::string> list_games();

    void leave_game(const std::string& name);
    Tateti& get_game(const std::string& name);


    GameMap(const GameMap&) = delete;
    GameMap& operator=(const GameMap&) = delete;

    GameMap(GameMap&&) = default;
    GameMap& operator=(GameMap&&) = default;

    ~GameMap() = default;
};

#endif
