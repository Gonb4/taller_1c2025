#include <cstring>
#include <exception>
#include <iostream>

#include "../common_src/setup_protocol.h"
#include "../common_src/constants.h"
#include "../common_src/stdio_manager.h"
#include "weapon_shop.h"
#include <memory>

int main(int argc, char* argv[]) {
    try {

        if (argc != 3) {
            std::cerr << "Bad program call. Expected " << argv[0]
                      << " <servname> <protocol-type>\n";
            return EXIT_FAILURE;
        }

        const std::string servname = argv[1];
        const std::string protocol_type = argv[2];
        uint8_t p_type = (protocol_type == "binary") ? BIN_PROTOCOL : TXT_PROTOCOL;

        SetupProtocol setup_p(servname);
        auto [protocol, username] = setup_p.wait_for_player(p_type);
        StdIOManager stdio_mngr;
        WeaponShop wpn_shop;

        stdio_mngr.print_player_welcome(username);

        while (not protocol->disconnected()) {
            PlayerInventory player_inv = wpn_shop.get_player_inventory();
            protocol->send_inventory(player_inv);
            auto [player_exit, transaction] = protocol->await_transaction();
            if (player_exit)
                break;
            bool transaction_confirmed = wpn_shop.process_transaction(transaction);
            if (not transaction_confirmed)
                stdio_mngr.print_transaction_rejected(transaction);

            // std::cout << transaction.type << ", " << transaction.wpn_name << ", " << transaction.wpn_type << ", " << transaction.ammo_qty << "\n";
        }
        // wait for player (receive username)
        // send protocol
        // setear variable protocolo
        // print username
        // crear inventario (clase PlayerInventory)
        // send inventory update

        // loop while(receive_transaction no devuelva algo que indice player desconectado):
        // receive transaccion (clase Transaction)
        // pasar Transaction a: PlayerInventory? clase Shop con atributo PlayerInventory? (devolver
        // algo que diga si fue valida o no) if not valida print no es valida
        // send inventory update


        return EXIT_SUCCESS;

    } catch (const std::exception& err) {std::cout << "EXCEPCION\n";}
}
