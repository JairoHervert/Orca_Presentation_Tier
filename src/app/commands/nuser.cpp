#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp" 

namespace client::cmd {

    bool run_nuser(const std::string& name, const std::string& email, const std::string& password) {
        std::cout << std::endl << "--- Registering New User ---" << std::endl;
        std::cout << "User:  " << name << std::endl;
        std::cout << "Email: " << email << std::endl;
 
        try {
            std::string hashedPassword = client::hasher_codec::hash_sha256(password);
            
            if (hashedPassword.empty()) {
                std::cerr << "Error hashing the password." << std::endl;
                return false;
            }
            
            // Creamos el payload 
            nlohmann::json payload = client::json_nlohmann::make_nuser_payload(name, email, hashedPassword);

            // Enviamos la petición
            std::cout << std::endl;
            nlohmann::json response = client::http::post_json_https("/user/create", payload);

            // Manejamos la respuesta
            client::response_handler::handle_nuser_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error registering user: " << e.what() << std::endl;
            return false;
        }
    }
}