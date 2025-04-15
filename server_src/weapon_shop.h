#ifndef WEAPON_SHOP_H
#define WEAPON_SHOP_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "../common_src/constants.h"

/**
 * @brief Tienda de armas. (Dueña del inventario del jugador)
 *
 * Clase encargada de procesar transacciones de compra de armas y munición,
 * manteniendo actualizado el inventario del jugador.
 */
class WeaponShop {
private:
    const std::unordered_map<std::string, Weapon>& weapon_map;
    PlayerInventory player_inv;

    bool process_weapon_purchase(const Transaction& t);
    bool process_ammo_purchase(const Transaction& t);

public:
    WeaponShop();

    /**
     * @brief Devuelve el inventario actual del jugador.
     *
     * @return Referencia constante al inventario del jugador.
     */
    const PlayerInventory& get_player_inventory() const;

    /**
     * @brief Procesa una transacción.
     *
     * Si la transacción es válida modifica su estado privado para actualizar el inventario del
     * jugador.
     *
     * @param t Transacción a procesar.
     * @return `true` si la transacción fue confirmada, `false` si fue rechazada.
     * @throws LibError si la transacción es de tipo `TransactionType::INVALID`.
     */
    bool process_transaction(const Transaction& t);

    WeaponShop(const WeaponShop&) = delete;
    WeaponShop& operator=(const WeaponShop&) = delete;

    WeaponShop(WeaponShop&&) = default;
    WeaponShop& operator=(WeaponShop&&) = default;

    ~WeaponShop() = default;
};

#endif
