#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    bool run_revoke(const std::string& project_name, const std::string& email, const std::string& file_target) {
        std::cout << "--- Revocando Permisos ---" << std::endl;
        std::cout << "Proyecto: " << project_name << std::endl;
        std::cout << "Email:    " << email << std::endl;
        
        if (file_target.empty()) {
            std::cout << "Alcance:  TODO EL PROYECTO" << std::endl;
        } else {
            std::cout << "Alcance:  Archivo especifico (" << file_target << ")" << std::endl;
        }

        try {
            nlohmann::json payload = client::json_nlohmann::make_revoke_payload(project_name, email, file_target);

            std::cout << "Enviando solicitud al servidor..." << std::endl;
            nlohmann::json response = client::http::post_json_https("/revoke", payload);

            client::response_handler::handle_revoke_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al revocar permisos: " << e.what() << std::endl;
            return false;
        }
    }

}