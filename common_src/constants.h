#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <vector>
#include <string>

#define HELLO_MSG 0x01
#define HELLO_MSG_SIZE 3  // sin incluir username
#define PROTOCOL_MSG 0x06
#define PROTOCOL_MSG_SIZE 2

#define BIN_PROTOCOL 0x07
#define TXT_PROTOCOL 0x08

#define WEAPON_PURCHASE_MSG 0x02
#define AMMO_PURCHASE_MSG 0x03
#define INVENTORY_MSG 0x07
#define INVENTORY_MSG_SIZE 10

#define STARTING_AMMO 30
#define KNIFE_EQUPD 0x01
// #define KNIFE_UNEQUP (cuchillo siempre equipado)

inline const std::string EQUIPPED_STR = "equipped";
inline const std::string NOT_EQUIPPED_STR = "not_equipped";

enum WeaponType {NONE, PRIMARY, SECONDARY};

struct Weapon {
    const std::string name;
    const uint8_t code;
    const WeaponType type;
    const int price;
    const int ammo_price;
};

inline const std::vector<Weapon> WEAPON_LIST = {
    {NOT_EQUIPPED_STR,    0x00,   NONE,       0,      0},
    {"glock",           0x01,   SECONDARY,  100,    1},
    {"ak-47",           0x02,   PRIMARY,    100,    1},
    {"m3",              0x03,   PRIMARY,    100,    1},
    {"awp",             0x04,   PRIMARY,    100,    1},
};

struct PlayerInventory {
    uint16_t money;
    std::string knife;
    std::string primary;
    uint16_t primary_ammo;
    std::string secondary;
    uint16_t secondary_ammo;

    PlayerInventory() :
        money(500), knife(EQUIPPED_STR), primary(NOT_EQUIPPED_STR), primary_ammo(0), secondary("glock"), secondary_ammo(STARTING_AMMO) {}
    
    PlayerInventory(uint16_t m, std::string k, std::string p, uint16_t p_ammo, std::string s, uint16_t s_ammo) :
        money(m), knife(k), primary(std::move(p)), primary_ammo(p_ammo), secondary(std::move(s)), secondary_ammo(s_ammo) {}

    // std::string as_string() const {}
};

enum TransactionType {WPN_PURCHASE, AMM_PURCHASE};

struct Transaction {
    const TransactionType type;

    Transaction(TransactionType t) : type(t) {}
};

struct WeaponPurchase : Transaction {
    const std::string wpn_name;

    WeaponPurchase(const std::string& wp_name) : Transaction(WPN_PURCHASE), wpn_name(wp_name) {}
};

struct AmmoPurchase : Transaction {
    const WeaponType wpn_type;
    const int amount;

    AmmoPurchase(WeaponType wp_type, int amount) : Transaction(AMM_PURCHASE), wpn_type(wp_type), amount(amount) {}
};

#endif
