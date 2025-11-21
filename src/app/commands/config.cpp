#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    bool run_config(const std::string& name, const std::string& email) {
        std::cout << "--- Configurando Usuario ---" << std::endl;
        std::cout << "Nombre: " << name << std::endl;
        std::cout << "Email:  " << email << std::endl;

        try {
            // 1. Crear el JSON
            nlohmann::json payload = client::json_nlohmann::make_config_payload(name, email);

            // 2. Enviar al servidor (POST /config)
            std::cout << "Enviando datos al servidor..." << std::endl;
            nlohmann::json response = client::http::post_json_https("/config", payload);

            // 3. Manejar la respuesta
            client::response_handler::handle_config_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al configurar usuario: " << e.what() << std::endl;
            return false;
        }
    }

}