#ifndef WEAPON_ENCODER_H
#define WEAPON_ENCODER_H

#include <cstdint>
#include <unordered_map>
#include <string>

class WeaponEncoder {
    private:
        std::unordered_map<std::string, uint8_t> name_to_code;
        std::unordered_map<uint8_t, std::string> code_to_name;
    
    public:
        WeaponEncoder();
    
        uint8_t ntoc(const std::string& name) const;

        std::string cton(uint8_t code) const;
    
        // std::string weapon_code_to_name(int code)
        // int weapon_name_to_code(std::string name)
    
        // client
    
        // server
    
        WeaponEncoder(const WeaponEncoder&) = delete;
        WeaponEncoder& operator=(const WeaponEncoder&) = delete;
    
        WeaponEncoder(WeaponEncoder&&) = default;
        WeaponEncoder& operator=(WeaponEncoder&&) = default;
    };

#endif
