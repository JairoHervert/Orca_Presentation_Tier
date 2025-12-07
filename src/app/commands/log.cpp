#include "client/commands.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    void run_log() {
        
        // FETCH COMMITS FROM SERVER
        nlohmann::json response = client::http::get_json_https("/repo/commits");

        // HANDLE RESPONSE
        client::response_handler::handle_commits_response(response);
    }
}