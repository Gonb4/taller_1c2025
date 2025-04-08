#include <cstring>
#include <exception>
#include <iostream>

#include "../common_src/bin_protocol.h"
#include "../common_src/constants.h"
#include "../common_src/txt_protocol.h"

int main(int argc, char* argv[]) {
    try {

        if (argc != 3) {
            std::cerr << "Bad program call. Expected " << argv[0]
                      << " <hostname> <servname> <username>\n";
            return EXIT_FAILURE;
        }

        const std::string servname = argv[1];
        // const char* p_type = argv[2];
        // const char protocol_type = (strcmp(p_type, "binary") == 0) ? BIN_PROTOCOL : TXT_PROTOCOL;

        BinaryProtocol protocol(servname);

        std::string username = protocol.wait_for_player();
        std::cout << username << " has arrived!\n";

        // WeaponShop wpn_shop;

        PlayerInventory player_inv;
        protocol.send_inventory(player_inv);

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

    } catch (const std::exception& err) {std::cout << "EXCEPCION\n";}
}
