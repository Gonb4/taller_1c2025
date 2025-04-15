#ifndef WEAPON_SHOP_H
#define WEAPON_SHOP_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "../common_src/constants.h"

class WeaponShop {
private:
    const std::unordered_map<std::string, Weapon>& weapon_map;
    PlayerInventory player_inv;

    bool process_weapon_purchase(const Transaction& t);
    bool process_ammo_purchase(const Transaction& t);

public:
    WeaponShop();

    const PlayerInventory& get_player_inventory() const;
    bool process_transaction(const Transaction& t);

    WeaponShop(const WeaponShop&) = delete;
    WeaponShop& operator=(const WeaponShop&) = delete;

    WeaponShop(WeaponShop&&) = default;
    WeaponShop& operator=(WeaponShop&&) = default;

    ~WeaponShop() = default;
};

#endif
