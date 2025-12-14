#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/json_codec.hpp"    
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp"
#include "client/response_handler.hpp"


namespace client::cmd {

    void run_list_files(const std::string& email, const std::string& password, const std::string& repo_name) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "--- Listing Repository Files ---" 
                  << client::colors::RESET << std::endl;
        
        std::cout << client::colors::YELLOW << "User:    " << client::colors::RESET << email << std::endl;
        std::cout << client::colors::YELLOW << "Project: " << client::colors::RESET << repo_name << std::endl;

        // hashear Password
        std::string hashedPassword = client::hasher_codec::hash_sha256(password);
        if (hashedPassword.empty()) {
            std::cerr << client::colors::RED << "[-] Error processing password." << client::colors::RESET << std::endl;
            return;
        }

        // Payload con user, pass y nombre del proyecto
        nlohmann::json payload = client::json_nlohmann::make_list_files_payload(email, hashedPassword, repo_name);

        // POST al endpoint de tu imagen
        nlohmann::json response = client::http::post_json_https("/repo/list_files", payload);

        // Pasamos 'repo_name' al handler para usarlo en el título
        client::response_handler::handle_list_files_response(response, repo_name);
    }
}