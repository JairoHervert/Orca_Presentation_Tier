#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    bool run_drop(const std::string& email) {
        std::cout << "--- Dando de Baja Usuario ---" << std::endl;
        std::cout << "Usuario (Email): " << email << std::endl;
        
        std::cout << "Estas seguro (Se enviara la peticion al servidor) [s/n]: ";
        char confirm;
        std::cin >> confirm;
        if (confirm != 's' && confirm != 'S') {
            std::cout << "Operacion cancelada." << std::endl;
            return false;
        }

        try {
            nlohmann::json payload = client::json_nlohmann::make_drop_payload(email);

            std::cout << "Enviando solicitud al servidor..." << std::endl;
            nlohmann::json response = client::http::post_json_https("/drop", payload);

            client::response_handler::handle_drop_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al dar de baja: " << e.what() << std::endl;
            return false;
        }
    }

}