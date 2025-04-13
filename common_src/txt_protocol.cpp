#include "txt_protocol.h"

TextProtocol::TextProtocol(Socket&& s):
        Protocol(std::move(s)) {}


// client
PlayerInventory TextProtocol::await_inventory_update() {
    return PlayerInventory();
}

// server
bool TextProtocol::player_disconnected() {
    return skt.is_stream_recv_closed();
}

void TextProtocol::send_inventory(const PlayerInventory&) {

}
