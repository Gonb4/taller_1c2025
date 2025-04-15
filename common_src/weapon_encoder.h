#ifndef WEAPON_ENCODER_H
#define WEAPON_ENCODER_H

#include <cstdint>
#include <string>
#include <unordered_map>

/**
 * @brief Clase encargada de codificar y decodificar armas.
 * 
 * Asocia nombres de armas con sus códigos numéricos y viceversa.
 */
class WeaponEncoder {
private:
    std::unordered_map<std::string, uint8_t> name_to_code;
    std::unordered_map<uint8_t, std::string> code_to_name;

public:
    WeaponEncoder();

    /**
     * @brief Convierte el nombre de un arma a su código.
     * 
     * @param name Nombre del arma.
     * @return Código correspondiente. Si el nombre del arma no existe devuelve el código asociado a NO_WEAPON_STR (0x00)
     */
    uint8_t ntoc(const std::string& name) const;

    /**
     * @brief Convierte un código de arma a su nombre.
     * 
     * @param code Código del arma.
     * @return Nombre correspondiente.
     */
    std::string cton(uint8_t code) const;

    WeaponEncoder(const WeaponEncoder&) = delete;
    WeaponEncoder& operator=(const WeaponEncoder&) = delete;

    WeaponEncoder(WeaponEncoder&&) = default;
    WeaponEncoder& operator=(WeaponEncoder&&) = default;

    ~WeaponEncoder() = default;
};

#endif
