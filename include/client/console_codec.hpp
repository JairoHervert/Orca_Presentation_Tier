#ifndef CONSOLE_CODEC_HPP
#define CONSOLE_CODEC_HPP

#include <string>

namespace client::console {

    // Solicita un password al usuario ocultando los caracteres
    std::string get_password_secure(const std::string& prompt);

    /**
     * Valida si la password cumple con:
     * - Minimo 8 caracteres.
     * - Al menos 1 letra.
     * - Al menos 1 numero.
     */
    bool validate_password_policy(const std::string& password);
}

#endif