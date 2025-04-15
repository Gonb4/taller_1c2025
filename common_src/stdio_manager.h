#ifndef SDTDIO_MANAGER_H
#define SDTDIO_MANAGER_H

#include <sstream>
#include <string>
#include <utility>

#include "constants.h"

/**
 * @brief Clase encargada de manejar la entrada y salida estándar del juego.
 * 
 * Esta clase abstrae la lógica de interacción con el usuario desde la terminal.
 * Se utiliza tanto del lado del cliente como del servidor.
 */
class StdIOManager {
private:
    // client
    std::pair<bool, Transaction> read_weapon_purchase(std::istringstream& iss);
    std::pair<bool, Transaction> read_ammo_purchase(std::istringstream& iss,
                                                    const PlayerInventory& p_inv);

    // server

public:
    StdIOManager();

    // client

    /**
     * @brief Imprime el inventario del jugador por salida estándar.
     * 
     * @param p_inv Inventario del jugador.
     */
    void print_inventory(const PlayerInventory& p_inv);

    /**
     * @brief Lee una operación ingresada por el usuario desde entrada estándar.
     * 
     * @param p_inv Inventario actual del jugador (para validaciones).
     * @return Un par:
     *         - `true` y una transacción de tipo `TransactionType::INVALID` si el usuario ingresó "exit".
     *         - `false` y una transacción válida en caso contrario.
     */
    std::pair<bool, Transaction> read_operation(const PlayerInventory& p_inv);

    // server

    /**
     * @brief Imprime un mensaje de bienvenida cuando se conecta un jugador.
     * 
     * @param username Nombre de usuario del jugador conectado.
     */
    void print_player_welcome(const std::string& username);

    /**
     * @brief Imprime un mensaje indicando que una transacción fue rechazada.
     * 
     * @throws LibError si la transacción `t` es de tipo `TransactionType::INVALID`.
     * 
     * @param t Transacción rechazada.
     */
    void print_transaction_rejected(const Transaction& t);


    StdIOManager(const StdIOManager&) = delete;
    StdIOManager& operator=(const StdIOManager&) = delete;

    StdIOManager(StdIOManager&&) = default;
    StdIOManager& operator=(StdIOManager&&) = default;

    ~StdIOManager() = default;
};

#endif
