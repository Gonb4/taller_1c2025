#include "bin_protocol.h"

#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>

BinaryProtocol::BinaryProtocol(Socket&& s):
        Protocol(std::move(s)), wpn_encoder() {}

        
// client        
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
    skt.sendall(buf.data(), buf.size());
}
