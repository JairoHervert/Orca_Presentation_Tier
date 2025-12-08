#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/hasher_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    void run_list_encrypted(const std::string& email, const std::string& password) {

        // hashear Password
        std::string hashedPassword = client::hasher_codec::hash_sha256(password);
        if (hashedPassword.empty()) {
            std::cerr << "[-] Error processing password." << std::endl;
            return;
        }
        
        // AHORA USAS LA FUNCIÓN DEL CODEC:
        nlohmann::json payload = client::json_nlohmann::make_list_encrypted_payload(email, hashedPassword);

        // Hacemos el POST
        nlohmann::json response = client::http::post_json_https("/repo/list_encrypted", payload);

        // Manejamos la respuesta
        client::response_handler::handle_list_encrypted_response(response);
    }
}