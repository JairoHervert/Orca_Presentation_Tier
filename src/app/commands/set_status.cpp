#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/hasher_codec.hpp" 
#include "client/client_https.hpp"
#include "client/response_handler.hpp"


namespace client::cmd {

    bool run_set_status(const std::string& approver_email, const std::string& approver_pass, const std::string& target_email, int new_status) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << " --- Changing User Status ---" 
                  << client::colors::RESET << std::endl;
        
        std::cout << client::colors::YELLOW << "Approver:   " << client::colors::RESET << approver_email << std::endl;
        std::cout << client::colors::YELLOW << "Target:     " << client::colors::RESET << target_email << std::endl;
        std::cout << client::colors::YELLOW << "New Status: " << client::colors::RESET << new_status << " (0=Inactive, 1=Active)" << std::endl;

        if (new_status < 0 || new_status > 1) {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error: Status must be 0 or 1." 
                      << client::colors::RESET << std::endl;
            return false;
        }
        
        try {
            // Hash-password
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_pass);

            // Crear Payload
            nlohmann::json payload = client::json_nlohmann::make_set_status_payload(approver_email, hashedPassword, target_email, new_status);

            nlohmann::json response = client::http::post_json_https("/user/change_status", payload);

            // Manejar respuesta
            client::response_handler::handle_set_status_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << client::colors::RED 
                      << "[-] Error: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }
}