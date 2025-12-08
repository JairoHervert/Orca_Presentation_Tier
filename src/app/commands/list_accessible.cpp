#include "client/commands.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/json_codec.hpp" 
#include <iostream>

namespace client::cmd {

    void run_list_accessible(const std::string& email, const std::string& password) {
        
        // 1. Crear Payload
        nlohmann::json payload = client::json_nlohmann::make_list_accessible_payload(email, password);

        // 2. Hacer POST
        nlohmann::json response = client::http::post_json_https("/repo/list_accessible", payload);

        // 3. Manejar Respuesta
        client::response_handler::handle_list_accessible_response(response);
    }
}