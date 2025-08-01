#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <utility>

#include "constants.h"
#include "socket.h"

/**
 * @brief Clase base abstracta para protocolos de comunicación.
 *
 * Esta clase define la interfaz común para todos los tipos de protocolo.
 * Las clases derivadas deben implementar todos los métodos virtuales puros.
 */
class Protocol {
protected:
    Socket skt;

    explicit Protocol(Socket&& s): skt(std::move(s)) {}

public:
    /**
     * @brief Indica si el socket fue desconectado.
     *
     * @return true si el socket fue cerrado; false en caso contrario.
     */
    virtual bool disconnected() = 0;

    // client

    /**
     * @brief Espera una actualización del inventario del jugador.
     *
     * @throws LibError si el socket se cierra inesperadamente.
     * @return PlayerInventory actualizado.
     */
    virtual PlayerInventory await_inventory_update() = 0;

    /**
     * @brief Solicita una transacción al servidor.
     *
     * @param t Transacción a enviar.
     * @throws LibError si la transacción es de tipo TransactionType::INVALID.
     */
    virtual void request_transaction(const Transaction& t) = 0;

    // server

    /**
     * @brief Envía el inventario del jugador al cliente.
     *
     * @param p_inv Inventario a enviar.
     * @throws LibError si el socket se cierra inesperadamente.
     */
    virtual void send_inventory(const PlayerInventory& p_inv) = 0;

    /**
     * @brief Espera una transacción del cliente.
     *
     * @return Un par:
     *         - `true` y una transacción de tipo `TransactionType::INVALID` si el cliente eligió
     * salir del juego
     *         - `false` y una transacción válida en caso contrario.
     */
    virtual std::pair<bool, Transaction> await_transaction() = 0;


    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    Protocol(Protocol&&) = default;
    Protocol& operator=(Protocol&&) = default;

    virtual ~Protocol() = default;
};

#endif
