#include "weapon_encoder.h"
#include "constants.h"

WeaponEncoder::WeaponEncoder() {
    for (const Weapon& w : WEAPON_LIST) {
        name_to_code[w.name] = w.code;
        code_to_name[w.code] = w.name;
    }
}
    
uint8_t WeaponEncoder::ntoc(const std::string& name) const {
    return name_to_code.at(name);
}

std::string WeaponEncoder::cton(uint8_t code) const {
    return code_to_name.at(code);
}
