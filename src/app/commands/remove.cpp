#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    bool run_remove(const std::string& project_name) {
        std::cout << "--- Eliminando Proyecto ---" << std::endl;
        std::cout << "Proyecto: " << project_name << std::endl;

        std::cout << "ADVERTENCIA: Esto borrara el proyecto del servidor permanentemente." << std::endl;
        std::cout << "Estas seguro [s/n]: ";
        char confirm;
        std::cin >> confirm;
        if (confirm != 's' && confirm != 'S') {
            std::cout << "Operacion cancelada." << std::endl;
            return false;
        }

        try {
            nlohmann::json payload = client::json_nlohmann::make_remove_payload(project_name);

            std::cout << "Enviando solicitud de borrado al servidor..." << std::endl;
            nlohmann::json response = client::http::post_json_https("/remove", payload);

            client::response_handler::handle_remove_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al eliminar repositorio: " << e.what() << std::endl;
            return false;
        }
    }

}