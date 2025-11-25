#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp" 

namespace client::cmd {

    bool run_nuser(const std::string& name, const std::string& email, const std::string& password) {
        std::cout << std::endl << "--- Registrando Nuevo Usuario ---" << std::endl;
        // ... (impresiones de info)

        try {
            // Usamos el nuevo módulo hasher_utils para hashear el string de password
            std::string hashedPassword = client::hasher::hash_sha256(password);
            
            if (hashedPassword.empty()) {
                std::cerr << "Error al hashear la contrasena." << std::endl;
                return false;
            }
            
            // Creamos el payload con la contraseña ya hasheada
            nlohmann::json payload = client::json_nlohmann::make_nuser_payload(name, email, hashedPassword);

            // Enviamos la petición
            std::cout <<std::endl;
            nlohmann::json response = client::http::post_json_https("/user/create", payload);

            // Manejamos la respuesta
            client::response_handler::handle_nuser_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al registrar usuario: " << e.what() << std::endl;
            return false;
        }
    }
}