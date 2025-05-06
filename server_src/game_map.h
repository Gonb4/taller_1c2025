#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <map>
#include <string>
#include <mutex>
#include <vector>

#include "tateti.h"

class GameMap {
    private:
        std::map<std::string, Tateti> games;
        std::map<std::string, int> player_count;
        std::mutex mtx;

    public:
        GameMap();
        // vvv creo que todos con mutex vvv
        bool create_game(const std::string& name);
        bool join_game(const std::string& name);
        std::vector<std::string> list_games();

        void leave_game(const std::string& name); 
        // void wipe_empty_games(); // NO HACE FALTA -> al final si (borrar los que player_count == 0)
        Tateti& get_game(const std::string& name); // este creo que no necesita mutex o quizas si por si acaso


        GameMap(const GameMap&) = delete;
        GameMap& operator=(const GameMap&) = delete;

        GameMap(GameMap&&) = default;
        GameMap& operator=(GameMap&&) = default;

        ~GameMap() = default;
};

#endif
