#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp"

namespace client::cmd {

    bool run_verify(const std::string& approver_email, const std::string& approver_password, const std::string& target_email) {
        std::cout << "--- Verificando Usuario Nuevo ---" << std::endl;
        std::cout << "Aprobador: " << approver_email << std::endl;
        std::cout << "Objetivo:  " << target_email << std::endl;
        
        try {
            // 1. Hashear la contraseña del aprobador (Seguridad)
            std::string hashedPassword = client::hasher::hash_sha256(approver_password);
            
            if (hashedPassword.empty()) {
                std::cerr << "Error interno al procesar la contrasena." << std::endl;
                return false;
            }

            // 2. Crear Payload
            nlohmann::json payload = client::json_nlohmann::make_verify_user_payload(approver_email, hashedPassword, target_email);

            // 3. Enviar (POST /user/verify_email)
            std::cout << "Enviando solicitud de verificacion..." << std::endl;
            nlohmann::json response = client::http::post_json_https("/user/verify_email", payload);

            // 4. Manejar Respuesta
            client::response_handler::handle_verify_response(response);
            
            // Retornar éxito si el status es ok
            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                return true;
            }
            return false;

        } catch (const std::exception &e) {
            std::cerr << "Error en la operacion de verificacion: " << e.what() << std::endl;
            return false;
        }
    }
}