#include "txt_protocol.h"
#include "liberror.h"
#include <sstream>

TextProtocol::TextProtocol(Socket&& s):
        Protocol(std::move(s)) {}


bool TextProtocol::disconnected() {
    return skt.is_stream_recv_closed();
}

std::istringstream TextProtocol::receive_message(int n) {
    char buf[MAX_TXT_PROTOCOL_MSG_LEN] = {0};
    char* idx = buf;
    int n_lines = 0;

    while (n_lines < n) {
        int r = skt.recvsome(idx, MAX_TXT_PROTOCOL_MSG_LEN - (idx - buf) - 1);
        if (r == 0)
            throw LibError(errno, "Server disconnected unexpectedly");
        for (char* i = idx; i < idx + r; ++i) {
            if (*i == '\n')
                n_lines += 1;
        }
        idx += r;  
    }

    return std::istringstream(buf);
}

// client
PlayerInventory TextProtocol::await_inventory_update() {
    std::istringstream iss = receive_message(N_LINES_INVENTORY_MSG);
    std::string line;

    std::getline(iss, line);
    uint16_t money = std::stoi(line.substr(line.find(':') + 1));

    std::getline(iss, line);
    std::string knife = (line.substr(line.find(':') + 1) == TRUE_STR) ? EQUIPPED_STR : NOT_EQUIPPED_STR;
    
    std::getline(iss, line);
    int pri_len = line.find(',') - line.find(':') - 1;
    std::string primary = line.substr(line.find(':') + 1, pri_len);
    uint16_t pri_ammo = std::stoi(line.substr(line.find(',') + 1));

    std::getline(iss, line);
    int sec_len = line.find(',') - line.find(':') - 1;
    std::string secondary = line.substr(line.find(':') + 1, sec_len);
    uint16_t sec_ammo = std::stoi(line.substr(line.find(',') + 1));

    return PlayerInventory(money, knife, primary, pri_ammo, secondary, sec_ammo);
}

void TextProtocol::request_transaction(const Transaction& t) {
    if (t.type == WPN_PURCHASE)
        request_weapon_purchase(t);
    else if (t.type == AMM_PURCHASE)
        request_ammo_purchase(t);
    else // t.type == TransactionType::INVALID
        throw LibError(errno, "Invalid Transaction used for parameter 't' in request_transaction");
}

void TextProtocol::request_weapon_purchase(const Transaction& t) {
    std::ostringstream request;
    request << WEAPON_PURCHASE_STR << ":" << t.wpn_name << "\n";

    auto buf = request.str();
    if (not skt.sendall(buf.data(), buf.size()))
        throw LibError(errno, "Server disconnected unexpectedly");
}

void TextProtocol::request_ammo_purchase(const Transaction& t) {
    std::ostringstream request;
    std::string wpn_type = (t.wpn_type == PRIMARY) ? PRIMARY_STR : SECONDARY_STR;
    request << AMMO_PURCHASE_STR << wpn_type << ":" << t.ammo_qty << "\n";
    
    auto buf = request.str();
    if (not skt.sendall(buf.data(), buf.size()))
        throw LibError(errno, "Server disconnected unexpectedly");
}

// server
void TextProtocol::send_inventory(const PlayerInventory& p_inv) {
    std::ostringstream update;
    std::string knife_eq = (p_inv.knife == EQUIPPED_STR) ? TRUE_STR : FALSE_STR;

    update
        << INV_MONEY_STR << ":" << p_inv.money << "\n"
        << INV_KNIFE_STR << ":" << knife_eq << "\n"
        << INV_PRIMARY_STR << ":" << p_inv.primary << "," << p_inv.primary_ammo << "\n"
        << INV_SECONDARY_STR << ":" << p_inv.secondary << "," << p_inv.secondary_ammo << "\n";

    auto buf = update.str();
    if (not skt.sendall(buf.data(), buf.size()))
        throw LibError(errno, "Player disconnected unexpectedly");
}

std::pair<bool, Transaction> TextProtocol::await_transaction() {
    try {
        std::istringstream iss = receive_message(N_LINES_TRANSACTION_MSG);
        std::string line;
        std::getline(iss, line);

        if (line.substr(0, line.find(':')) == WEAPON_PURCHASE_STR)
            return await_weapon_purchase(line);
        else
            return await_ammo_purchase(line);

    } catch (const std::exception& err) {
        return {true, Transaction()};
    }
}

std::pair<bool, Transaction> TextProtocol::await_weapon_purchase(const std::string& line) {
    std::string wpn_name = line.substr(line.find(':') + 1);

    return {false, WeaponPurchase(std::move(wpn_name))};
}

std::pair<bool, Transaction> TextProtocol::await_ammo_purchase(const std::string& line) {
    int w_t_len = line.find(':') - line.rfind('.') - 1;
    std::string w_t = line.substr(line.rfind('.') + 1, w_t_len);
    uint16_t amount = std::stoi(line.substr(line.find(':') + 1));

    if (w_t == PRIMARY_STR)
        return {false, AmmoPurchase(PRIMARY, amount)};
    else // (w_t == SECONDARY_STR)
        return {false, AmmoPurchase(SECONDARY, amount)};
}
