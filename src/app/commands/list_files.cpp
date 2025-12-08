#include "client/commands.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/json_codec.hpp" 

namespace client::cmd {

    void run_list_files(const std::string& email, const std::string& password, const std::string& repo_name) {
        
        // Payload con user, pass y nombre del proyecto
        nlohmann::json payload = client::json_nlohmann::make_list_files_payload(email, password, repo_name);

        // POST al endpoint de tu imagen
        nlohmann::json response = client::http::post_json_https("/repo/list_files", payload);

        // Pasamos 'repo_name' al handler para usarlo en el título
        client::response_handler::handle_list_files_response(response, repo_name);
    }
}