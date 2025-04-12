#include <cstring>
#include <exception>
#include <iostream>

#include "../common_src/setup_protocol.h"
#include "../common_src/constants.h"
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
        // std::string username = setup_p.receive_username(); //ERROR aca setup_p ya no tiene el socket

        std::cout << username << " has arrived!\n";

        // WeaponShop wpn_shop;

        PlayerInventory player_inv;
        protocol->send_inventory(player_inv);

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
