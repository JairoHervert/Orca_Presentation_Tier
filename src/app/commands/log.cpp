#include "client/commands.hpp"
#include <iostream>
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    bool run_log(const std::string& project_name) {
        std::cout << "Consultando historial para: " << project_name << "..." << std::endl;

        try {
            nlohmann::json payload = client::json_nlohmann::make_log_payload(project_name);

            nlohmann::json response = client::http::post_json_https("/log", payload);

            // 3. Manejar respuesta visualmente
            client::response_handler::handle_log_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al consultar historial: " << e.what() << std::endl;
            return false;
        }
    }

} 