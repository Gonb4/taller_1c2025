#include <exception>
#include <iostream>
#include <memory>

#include "../common_src/constants.h"
#include "../common_src/setup_protocol.h"
#include "../common_src/stdio_manager.h"

int main(int argc, const char* argv[]) {
    try {

        if (argc != 4) {
            std::cerr << "Bad program call. Expected " << argv[0]
                      << " <hostname> <servname> <username>\n";
            return EXIT_FAILURE;
        }

        const std::string hostname = argv[1];
        const std::string servname = argv[2];
        const std::string username = argv[3];

        SetupProtocol setup_p(hostname, servname);
        std::unique_ptr<Protocol> protocol = setup_p.request_login(username);
        StdIOManager stdio_mngr;

        while (not protocol->disconnected()) {
            PlayerInventory player_inv = protocol->await_inventory_update();
            stdio_mngr.print_inventory(player_inv);
            auto [exit, transaction] = stdio_mngr.read_operation(player_inv);
            if (exit)
                break;
            protocol->request_transaction(transaction);
        }

        return EXIT_SUCCESS;

    } catch (const std::exception& err) {
        std::cerr << "Exception: " << err.what() << "\n";

        return EXIT_FAILURE;
    }
}
