#include "stdio_manager.h"

#include <iostream>


StdIOManager::StdIOManager() {}

// client
void StdIOManager::print_inventory(const PlayerInventory& p_inv) {
    std::ostringstream inv_msg;

    inv_msg << "money: $"     << p_inv.money     << " | "
            << "knife: "      << p_inv.knife     << " | "
            << "primary: "    << p_inv.primary;
    if (p_inv.primary != NOT_EQUIPPED_STR)
        inv_msg << ", " << p_inv.primary_ammo;
    inv_msg << " | "
            << "secondary: "  << p_inv.secondary;
    if (p_inv.secondary != NOT_EQUIPPED_STR)
        inv_msg << ", " << p_inv.secondary_ammo;
    inv_msg << "\n";

    std::cout << inv_msg.str();
}

std::pair<bool, Transaction> StdIOManager::read_operation(const PlayerInventory& p_inv) {
    std::string line;
    std::string command;

    while (true) {
        std::getline(std::cin, line);
        std::istringstream iss(line);
        iss >> command;

        if (command == WEAPON_PURCHASE_CMD) {
            auto [valid, t] = read_weapon_purchase(iss);
            if (valid)
                return {false, t};
        } else if (command == AMMO_PURCHASE_CMD) {
            auto [valid, t] = read_ammo_purchase(iss, p_inv);
            if (valid)
                return {false, t};
        } else if (command == EXIT_CMD) {
            return {true, Transaction()};
        }
    }
}

std::pair<bool, Transaction> StdIOManager::read_weapon_purchase(std::istringstream& iss) {
    std::string wpn_name;

    if ((iss >> wpn_name) && wpn_name != NOT_EQUIPPED_STR) // chequea que haya al menos 1 palabra mas
        return {true, WeaponPurchase(std::move(wpn_name))};

    return {false, Transaction()};
}

std::pair<bool, Transaction> StdIOManager::read_ammo_purchase(std::istringstream& iss, const PlayerInventory& p_inv) {
    std::string wpn_name;
    int amount;

    if ((iss >> wpn_name >> amount) && amount > 0 && amount < MAX_AMMO && wpn_name != NOT_EQUIPPED_STR) { // chequea que haya al menos 2 palabras mas y la 2da debe poder guardarse como int
        if (wpn_name == p_inv.primary)
            return {true, AmmoPurchase(PRIMARY, amount)};
        if (wpn_name == p_inv.secondary)
            return {true, AmmoPurchase(SECONDARY, amount)};
    }

    return {false, Transaction()};
}

// server
void StdIOManager::print_player_welcome(std::string& username) {
    std::ostringstream welcome_msg;
    welcome_msg << username << " has arrived!\n";

    std::cout << welcome_msg.str();
}

void StdIOManager::print_invalid_transaction() {

}

void StdIOManager::print_invalid_weapon_purchase() {

}

void StdIOManager::print_invalid_ammo_purchase() {
    
}
