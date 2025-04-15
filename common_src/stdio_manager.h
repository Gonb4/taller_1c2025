#ifndef SDTDIO_MANAGER_H
#define SDTDIO_MANAGER_H

#include <sstream>
#include <string>
#include <utility>

#include "constants.h"


class StdIOManager {
private:
    // client
    std::pair<bool, Transaction> read_weapon_purchase(std::istringstream& iss);
    std::pair<bool, Transaction> read_ammo_purchase(std::istringstream& iss,
                                                    const PlayerInventory& p_inv);

    // server
    // void print_weapon_purchase_rejected(const Transaction& t);
    // void print_ammo_purchase_rejected(const Transaction& t);

public:
    StdIOManager();

    // client
    void print_inventory(const PlayerInventory& p_inv);
    std::pair<bool, Transaction> read_operation(const PlayerInventory& p_inv);

    // server
    void print_player_welcome(const std::string& username);
    void print_transaction_rejected(const Transaction& t);


    StdIOManager(const StdIOManager&) = delete;
    StdIOManager& operator=(const StdIOManager&) = delete;

    StdIOManager(StdIOManager&&) = default;
    StdIOManager& operator=(StdIOManager&&) = default;

    ~StdIOManager() = default;
};

#endif
