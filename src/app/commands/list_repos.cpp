#include "client/commands.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    void run_list_repos() {
        
        nlohmann::json response = client::http::get_json_https("/repo/list_all");

        client::response_handler::handle_list_repos_response(response);
    }
}