#include "weapon_encoder.h"
#include "constants.h"

WeaponEncoder::WeaponEncoder() {
    for (const auto& [n, w] : WEAPON_MAP) {
        name_to_code[w.name] = w.code;
        code_to_name[w.code] = w.name;
    }
}
    
uint8_t WeaponEncoder::ntoc(const std::string& name) const {
    if (name_to_code.count(name))
        return name_to_code.at(name);
    else
        return name_to_code.at(NOT_EQUIPPED_STR);
}

std::string WeaponEncoder::cton(uint8_t code) const {
    return code_to_name.at(code);
}
