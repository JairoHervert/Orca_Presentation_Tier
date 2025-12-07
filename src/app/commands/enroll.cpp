#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp" // Para hashear password

namespace client::cmd {

    bool run_enroll(const std::string& approver_email, const std::string& approver_password, const std::string& project_name, const std::string& target_email) {
        std::cout << "\n --- Agregando Colaborador a Proyecto ---" << std::endl;
        std::cout << "Proyecto:  " << project_name << std::endl;
        std::cout << "Aprobador: " << approver_email << std::endl;
        std::cout << "Nuevo:     " << target_email << std::endl;

        try {
            // Hashear password del aprobador
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_password);
            if (hashedPassword.empty()) {
                std::cerr << "Error interno al procesar password." << std::endl;
                return false;
            }

            // Crear Payload
            nlohmann::json payload = client::json_nlohmann::make_enroll_payload(approver_email, hashedPassword, project_name, target_email);

            nlohmann::json response = client::http::post_json_https("/repo/add_user", payload);

            // Mostrar respuesta
            client::response_handler::handle_enroll_response(response);

            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                return true;
            }
            return false;

        } catch (const std::exception &e) {
            std::cerr << "Error en enroll: " << e.what() << std::endl;
            return false;
        }
    }

}
