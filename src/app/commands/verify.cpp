#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp"

namespace client::cmd {

    bool run_verify(const std::string& approver_email, const std::string& approver_password, const std::string& target_email) {
        std::cout << "\n --- Verifying New User ---" << std::endl;
        std::cout << "Approver: " << approver_email << std::endl;
        std::cout << "Target:   " << target_email << std::endl;
        
        try {
            // Hashear la contraseña del aprobador (Seguridad)
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_password);
            
            if (hashedPassword.empty()) {
                std::cerr << "\n[-] Internal error processing password." << std::endl;
                return false;
            }

            // Crear Payload
            nlohmann::json payload = client::json_nlohmann::make_verify_user_payload(approver_email, hashedPassword, target_email);

            nlohmann::json response = client::http::post_json_https("/user/verify_email", payload);

            // Manejar Respuesta
            client::response_handler::handle_verify_response(response);
            
            // Retornar éxito si el status es ok
            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                return true;
            }
            return false;

        } catch (const std::exception &e) {
            std::cerr << "\n[-] Error in verification operation: " << e.what() << std::endl;
            return false;
        }
    }
}