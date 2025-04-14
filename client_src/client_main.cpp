#include <exception>
#include <iostream>
#include <memory>

#include "../common_src/setup_protocol.h"
#include "../common_src/constants.h"
#include "../common_src/stdio_manager.h"

int main(int argc, char* argv[]) {
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
        // money: $500 | knife: equipped | primary: not_equipped | secondary: glock, 30
        
        // enter lobby (send username)
        // receive protocol
        // setear variable protocol_type
        // receive inventory update
        // print inventory

        // loop while(getline stdin (parse) no sea exit):
        // if exit return
        //  -weapon purchase
        //  -ammo purchase
        // receive inventory update
        // print inventory


        return EXIT_SUCCESS;
        
    } catch (const std::exception& err) {std::cout << "EXCEPCION\n";}
}
