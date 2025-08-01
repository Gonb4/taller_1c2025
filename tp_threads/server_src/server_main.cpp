#include <exception>
#include <iostream>
#include <string>

#include "../common_src/stdio_manager.h"

#include "acceptor_thread.h"
#include "game_map.h"

int main(int argc, const char* argv[]) {
    try {

        if (argc != 2) {
            std::cerr << "Bad program call. Expected: " << argv[0] << " <port>\n";
            return EXIT_FAILURE;
        }

        const std::string port = argv[1];

        GameMap game_map;
        AcceptorThread acc_th(port, game_map);
        acc_th.start();
        StdIOManager stdio_mngr;

        stdio_mngr.read_quit();

        acc_th.force_stop();
        acc_th.join();

        return EXIT_SUCCESS;

    } catch (const std::exception& err) {
        std::cerr << "Exception: " << err.what() << "\n";
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
    }

    return EXIT_FAILURE;
}
