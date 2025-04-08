#include <exception>
#include <iostream>

#include "../common_src/bin_protocol.h"
#include "../common_src/constants.h"
#include "../common_src/txt_protocol.h"

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

        BinaryProtocol protocol(hostname, servname);

        int p = protocol.enter_lobby(username);
        const char* protocol_type = (p == BIN_PROTOCOL) ? "BIN_PROTOCOL" : "TXT_PROTOCOL";
        std::cout << protocol_type << "\n";

        PlayerInventory player_inv = protocol.await_inventory_update();

        std::cout << "money: $"     << player_inv.money     << " | "
                  << "knife: "      << player_inv.knife     << " | "
                  << "primary: "    << player_inv.primary;
        if (player_inv.primary != NOT_EQUIPPED_STR)
            std::cout << ", " << player_inv.primary_ammo;
        std::cout << " | "
                  << "secondary: "  << player_inv.secondary;
        if (player_inv.secondary != NOT_EQUIPPED_STR)
            std::cout << ", " << player_inv.secondary_ammo;
        std::cout << "\n";
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

    } catch (const std::exception& err) {std::cout << "EXCEPCION\n";}
}
