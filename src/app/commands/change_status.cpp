#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp" 

namespace client::cmd {

    bool run_change_status(const std::string& approver_email, const std::string& approver_pass, const std::string& target_email, int new_status) {
        std::cout << "\n --- Cambiando Estatus de Usuario ---" << std::endl;
        std::cout << "Aprobador: " << approver_email << std::endl;
        std::cout << "Objetivo:  " << target_email << std::endl;
        std::cout << "Nuevo Status: " << new_status << " (0=Inactive, 1=Active)" << std::endl;

        if (new_status < 0 || new_status > 1) {
            std::cerr << "Error: El status debe ser 0 o 1." << std::endl;
            return false;
        }
        
        try {
            // Hashear password
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_pass);

            // Crear Payload
            nlohmann::json payload = client::json_nlohmann::make_change_status_payload(approver_email, hashedPassword, target_email, new_status);

            nlohmann::json response = client::http::post_json_https("/user/change_status", payload);

            // Manejar respuesta
            client::response_handler::handle_change_status_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }
}