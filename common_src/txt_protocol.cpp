#include "txt_protocol.h"

TextProtocol::TextProtocol(Socket&& s):
        Protocol(std::move(s)) {}


// client
PlayerInventory TextProtocol::await_inventory_update() {
    return PlayerInventory();
}

// server
void TextProtocol::send_inventory(const PlayerInventory&) {

}
