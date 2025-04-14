#include "txt_protocol.h"

TextProtocol::TextProtocol(Socket&& s):
        Protocol(std::move(s)) {}


bool TextProtocol::disconnected() {
    return skt.is_stream_recv_closed();
}

// client
PlayerInventory TextProtocol::await_inventory_update() {
    return PlayerInventory();
}

void TextProtocol::request_transaction(const Transaction& t) {

}

void TextProtocol::request_weapon_purchase(const Transaction& t) {

} //(agregar constante "buy")

void TextProtocol::request_ammo_purchase(const Transaction& t) {
    
} //(agregar constante "ammo")

// server
void TextProtocol::send_inventory(const PlayerInventory&) {

}

std::pair<bool, Transaction> TextProtocol::await_transaction() {
    return {true, Transaction()};
}

std::pair<bool, Transaction> TextProtocol::await_weapon_purchase() {
    return {true, Transaction()};
}

std::pair<bool, Transaction> TextProtocol::await_ammo_purchase() {
    return {true, Transaction()};
}
