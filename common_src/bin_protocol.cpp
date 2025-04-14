#include "bin_protocol.h"

#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include "liberror.h"

BinaryProtocol::BinaryProtocol(Socket&& s):
        Protocol(std::move(s)), wpn_encoder() {}


bool BinaryProtocol::disconnected() {
    return skt.is_stream_recv_closed();
}

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

void BinaryProtocol::request_transaction(const Transaction& t) {
    if (t.type == WPN_PURCHASE)
        request_weapon_purchase(t);
    else if (t.type == AMM_PURCHASE)
        request_ammo_purchase(t);
    else // t.type == TransactionType::INVALID
        throw LibError(errno, "Invalid Transaction used for parameter 't' in request_transaction");
}

void BinaryProtocol::request_weapon_purchase(const Transaction& t) {
    std::ostringstream request;
    request.put(WEAPON_PURCHASE_MSG);
    request.put(wpn_encoder.ntoc(t.wpn_name)); // si t.wpn_name es invalido el code es 0x00 (arma NOT_EQUIPPED_STR)

    auto buf = request.str();
    skt.sendall(buf.data(), buf.size());
}

void BinaryProtocol::request_ammo_purchase(const Transaction& t) {
    std::ostringstream request;
    request.put(AMMO_PURCHASE_MSG);
    request.put(t.wpn_type);

    uint16_t amount = htons(t.ammo_qty);
    request.write((const char*)&amount, sizeof(amount));

    auto buf = request.str();
    skt.sendall(buf.data(), buf.size());
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

Transaction BinaryProtocol::await_transaction() {
    uint8_t type;
    skt.recvall(&type, 1);

    if (type == WEAPON_PURCHASE_MSG)
        return await_weapon_purchase();
    else // (type == AMMO_PURCHASE_MSG)
        return await_ammo_purchase();
}

Transaction BinaryProtocol::await_weapon_purchase() {
    uint8_t buf[WEAPON_PURCHASE_MSG_SIZE - 1];
    skt.recvall(buf, WEAPON_PURCHASE_MSG_SIZE - 1);

    uint8_t wpn_code = *buf;

    return WeaponPurchase(wpn_encoder.cton(wpn_code));
}

Transaction BinaryProtocol::await_ammo_purchase() {
    uint8_t buf[AMMO_PURCHASE_MSG_SIZE - 1];
    skt.recvall(buf, AMMO_PURCHASE_MSG_SIZE - 1);

    WeaponType wpn_type = (WeaponType)*buf;
    uint16_t amount = ntohs(*(uint16_t*)(buf+1));

    return AmmoPurchase(wpn_type, amount);
}
