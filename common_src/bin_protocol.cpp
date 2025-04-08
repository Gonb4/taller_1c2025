#include "bin_protocol.h"

#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "constants.h"

// client
BinaryProtocol::BinaryProtocol(const std::string& hostname, const std::string& servname):
        peer_skt(hostname.c_str(), servname.c_str()), skt(std::move(peer_skt)), wpn_encoder() {} //construyo peer_socket y lo anulo moviendolo a skt

uint8_t BinaryProtocol::enter_lobby(const std::string& username) {
    std::ostringstream request;
    request.put(HELLO_MSG);

    uint16_t username_len = htons(username.size());
    request.write(reinterpret_cast<const char*>(&username_len), sizeof(username_len));

    request << username;

    auto buf = request.str();
    skt.sendall(buf.data(), buf.size());

    uint8_t response_buf[PROTOCOL_MSG_SIZE];
    skt.recvall(response_buf, PROTOCOL_MSG_SIZE);

    return response_buf[1];
}

PlayerInventory BinaryProtocol::await_inventory_update() {
    uint8_t buf[INVENTORY_MSG_SIZE];
    skt.recvall(buf, INVENTORY_MSG_SIZE);
    const uint8_t* i = buf + 1;

    uint16_t money = ntohs(*(uint16_t*)i);
    i += sizeof(money);
    std::string knife = (*i == KNIFE_EQUPD) ? EQUIPPED_STR : NOT_EQUIPPED_STR;
    i += 1;
    
    uint8_t pri_code = *i;
    i += sizeof(pri_code);
    std::string primary = wpn_encoder.cton(pri_code);
    uint16_t pri_ammo = ntohs(*(uint16_t*)i);
    i += sizeof(pri_ammo);

    uint8_t sec_code = *i;
    i += sizeof(sec_code);
    std::string secondary = wpn_encoder.cton(sec_code);
    uint16_t sec_ammo = ntohs(*(uint16_t*)i);
    i += sizeof(sec_ammo);

    return PlayerInventory(money, knife, primary, pri_ammo, secondary, sec_ammo);
}

// server
BinaryProtocol::BinaryProtocol(const std::string& servname):
        peer_skt(servname.c_str()), skt(std::move(peer_skt)), wpn_encoder() {} //construyo peer_socket y lo anulo moviendolo a skt

std::string BinaryProtocol::wait_for_player() {
    this->peer_skt = skt.accept(); //revivo peer_socket

    uint8_t buf[HELLO_MSG_SIZE];
    peer_skt.recvall(buf, HELLO_MSG_SIZE);

    uint16_t username_len = ntohs(*(uint16_t*)(buf + 1));
    std::string username(username_len, '\0');
    peer_skt.recvall(username.data(), username_len);

    std::ostringstream response;
    response.put(PROTOCOL_MSG);
    response.put(BIN_PROTOCOL);

    auto response_buf = response.str();
    peer_skt.sendall(response_buf.data(), response_buf.size());

    return username;
}

void BinaryProtocol::send_inventory(const PlayerInventory& p_inv) {
    std::ostringstream update;
    update.put(INVENTORY_MSG);

    uint16_t money = htons(p_inv.money);
    update.write((const char*)&money, sizeof(money));
   
    if (p_inv.knife == EQUIPPED_STR)
        update.put(KNIFE_EQUPD);
    // else... (cuchillo siempre equipado)
    
    update.put(wpn_encoder.ntoc(p_inv.primary));
    uint16_t pri_ammo = htons(p_inv.primary_ammo);
    update.write((const char*)&pri_ammo, sizeof(pri_ammo));
    
    update.put(wpn_encoder.ntoc(p_inv.secondary));
    uint16_t sec_ammo = htons(p_inv.secondary_ammo);
    update.write((const char*)&sec_ammo, sizeof(sec_ammo));

    auto buf = update.str();
    peer_skt.sendall(buf.data(), buf.size());
}
