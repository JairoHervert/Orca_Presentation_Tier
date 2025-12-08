#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    void run_list_encrypted(const std::string& email, const std::string& password) {
        
        // AHORA USAS LA FUNCIÓN DEL CODEC:
        nlohmann::json payload = client::json_nlohmann::make_list_encrypted_payload(email, password);

        // Hacemos el POST
        nlohmann::json response = client::http::post_json_https("/repo/list_encrypted", payload);

        // Manejamos la respuesta
        client::response_handler::handle_list_encrypted_response(response);
    }
}