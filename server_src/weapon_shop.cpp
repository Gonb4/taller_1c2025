#include "weapon_shop.h"

#include "../common_src/liberror.h"

WeaponShop::WeaponShop(): weapon_map(WEAPON_MAP), player_inv(PlayerInventory()) {}


const PlayerInventory& WeaponShop::get_player_inventory() const { return player_inv; }

bool WeaponShop::process_transaction(const Transaction& t) {
    if (t.type == WPN_PURCHASE)
        return process_weapon_purchase(t);
    else if (t.type == AMM_PURCHASE)
        return process_ammo_purchase(t);
    else  // t.type == TransactionType::INVALID
        throw LibError(errno, "Invalid Transaction used for parameter 't' in request_transaction");
}

bool WeaponShop::process_weapon_purchase(const Transaction& t) {
    if (t.wpn_name == NO_WEAPON_STR)
        return true;  // transaccion confirmada pero sin efecto (arma inexistente) (protocolo
                      // binario)
    if (not weapon_map.count(t.wpn_name))
        return true;  // transaccion confirmada pero sin efecto (arma inexistente) (protocolo texto)

    Weapon weapon = weapon_map.at(t.wpn_name);
    int cost = weapon.price;
    if (player_inv.money < cost)
        return false;

    if (weapon.type == PRIMARY) {
        player_inv.primary = weapon.name;
        player_inv.primary_ammo = STARTING_AMMO;
    } else {  // weapon.type == SECONDARY
        player_inv.secondary = weapon.name;
        player_inv.secondary_ammo = STARTING_AMMO;
    }
    player_inv.money -= cost;

    return true;
}

bool WeaponShop::process_ammo_purchase(const Transaction& t) {
    int cost;
    if (t.wpn_type == PRIMARY) {
        Weapon weapon = weapon_map.at(player_inv.primary);
        cost = weapon.ammo_price * t.ammo_qty;
        if (player_inv.money < cost)
            return false;
        player_inv.primary_ammo += t.ammo_qty;
    } else {  // t.wpn_type == SECONDARY
        Weapon weapon = weapon_map.at(player_inv.secondary);
        cost = weapon.ammo_price * t.ammo_qty;
        if (player_inv.money < cost)
            return false;
        player_inv.secondary_ammo += t.ammo_qty;
    }
    player_inv.money -= cost;

    return true;
}
