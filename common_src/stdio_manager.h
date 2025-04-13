#ifndef SDTDIO_MANAGER_H
#define SDTDIO_MANAGER_H

#include <string>
#include <sstream>
#include "constants.h"


class StdIOManager {
private:
    // client
    std::pair<bool, Transaction> read_weapon_purchase(std::istringstream& iss);
    std::pair<bool, Transaction> read_ammo_purchase(std::istringstream& iss);

    // server
    void print_invalid_weapon_purchase();
    void print_invalid_ammo_purchase();

public:
    StdIOManager();

    // client
    void print_inventory(PlayerInventory& p_inv);
    std::pair<bool, Transaction> read_operation();

    //request_transaction
    //request_weapon_purchase (agregar constante "buy")
    //request_ammo_purchase (agregar constante "ammo")
    
    // server
    void print_player_welcome(std::string& username);
    void print_invalid_transaction();
    //await_transaction (devuelve Transaction, llama a las de abajo)
    //-await_weapon_purchase (privado?)
    //-await_ammo_purchase (privado?)

    StdIOManager(const StdIOManager&) = delete;
    StdIOManager& operator=(const StdIOManager&) = delete;

    StdIOManager(StdIOManager&&) = default;
    StdIOManager& operator=(StdIOManager&&) = default;

    ~StdIOManager() = default;
};

#endif