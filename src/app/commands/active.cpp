#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    bool run_active(const std::string& email) {
        std::cout << "--- Activando Usuario ---" << std::endl;
        std::cout << "Usuario (Email): " << email << std::endl;

        std::cout << "¿Estas seguro de reactivar a este usuario? [s/n]: ";
        char confirm;
        std::cin >> confirm;
        if (confirm != 's' && confirm != 'S') {
            std::cout << "Operacion cancelada." << std::endl;
            return false;
        }

        try {
            nlohmann::json payload = client::json_nlohmann::make_active_payload(email);

            std::cout << "Enviando solicitud al servidor..." << std::endl;
            nlohmann::json response = client::http::post_json_https("/active", payload);

            client::response_handler::handle_active_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al activar usuario: " << e.what() << std::endl;
            return false;
        }
    }

}