#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp" 

namespace client::cmd {

    bool run_change_role(const std::string& approver_email, const std::string& approver_password, const std::string& target_email, int new_role) {
        std::cout << "\n --- Cambiando Nivel de Usuario ---" << std::endl;
        std::cout << "Aprobador: " << approver_email << std::endl;
        std::cout << "Objetivo:  " << target_email << std::endl;
        std::cout << "Nuevo Rol: " << new_role << " (1=Dev, 2=Lead, 3=Sen)" << std::endl;
        
        if (new_role < 1 || new_role > 3) {
            std::cerr << "Error: El rol debe ser 1, 2 o 3." << std::endl;
            return false;
        }

        try {
            // Hashear password 
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_password);
            
            if (hashedPassword.empty()) {
                std::cerr << "Error interno al procesar la contrasena." << std::endl;
                return false;
            }

            nlohmann::json payload = client::json_nlohmann::make_change_role_payload(approver_email, hashedPassword, target_email, new_role);


            nlohmann::json response = client::http::post_json_https("/user/change_level", payload);

            client::response_handler::handle_change_role_response(response);
            
            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                return true;
            }
            return false;

        } catch (const std::exception &e) {
            std::cerr << " Error en la operacion: " << e.what() << std::endl;
            return false;
        }
    }
}