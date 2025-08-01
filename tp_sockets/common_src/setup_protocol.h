#ifndef SETUP_PROTOCOL_H
#define SETUP_PROTOCOL_H

#include <memory>
#include <string>
#include <utility>

#include "bin_protocol.h"
#include "socket.h"
#include "txt_protocol.h"

/**
 * @brief Clase encargada de establecer la conexión inicial entre cliente y servidor.
 *
 * SetupProtocol se encarga del protocolo de conexión y autenticación previo
 * al inicio del protocolo de comunicación principal (Binary o Text Protocol).
 * Del lado del servidor conserva el Socket aceptador (lo que posibilitaría
 * aceptar a varios clientes).
 */
class SetupProtocol {
private:
    Socket skt;

public:
    // client
    SetupProtocol(const std::string& hostname, const std::string& servname);

    /**
     * @brief Solicita la conexión con el servidor.
     * @param username Nombre de usuario que se desea utilizar.
     * @return Un std::unique_ptr<Protocol> del tipo adecuado para la comunicación.
     */
    std::unique_ptr<Protocol> request_login(const std::string& username);


    // server
    explicit SetupProtocol(const std::string& servname);

    /**
     * @brief Espera la conexión de un cliente y lo acepta.
     * @param p_type Tipo de protocolo a utilizar (BIN_PROTOCOL o TXT_PROTOCOL).
     * @return Un par con un std::unique_ptr<Protocol> al protocolo correspondiente y el nombre de
     * usuario recibido.
     */
    std::pair<std::unique_ptr<Protocol>, std::string> wait_for_player(const uint8_t p_type);


    SetupProtocol(const SetupProtocol&) = delete;
    SetupProtocol& operator=(const SetupProtocol&) = delete;

    SetupProtocol(SetupProtocol&&) = default;
    SetupProtocol& operator=(SetupProtocol&&) = default;

    ~SetupProtocol() = default;
};

#endif
