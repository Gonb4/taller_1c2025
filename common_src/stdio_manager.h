#ifndef STDIO_MANAGER_H
#define STDIO_MANAGER_H

#include <string>

#include "constants.h"

class StdIOManager {
    public:
        StdIOManager();

        // ======================= CLIENT =======================

        Operation read_operation();
        PlayerMove read_game_move();
        void print(const std::string& msg);

        // ======================= SERVER =======================

        void read_quit();


        StdIOManager(const StdIOManager&) = delete;
        StdIOManager& operator=(const StdIOManager&) = delete;

        StdIOManager(StdIOManager&&) = default;
        StdIOManager& operator=(StdIOManager&&) = default;

        ~StdIOManager() = default;
};

#endif
