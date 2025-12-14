#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp" 
#include "client/response_handler.hpp"


namespace client::cmd {

    bool run_set_role(const std::string& approver_email, const std::string& approver_password, const std::string& target_email, int new_role) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << " --- Changing User Level ---" 
                  << client::colors::RESET << std::endl;
        
        std::cout << client::colors::YELLOW << "Approver: " << client::colors::RESET << approver_email << std::endl;
        std::cout << client::colors::YELLOW << "Target:   " << client::colors::RESET << target_email << std::endl;
        std::cout << client::colors::YELLOW << "New Role: " << client::colors::RESET << new_role << " (1=Dev, 2=Lead, 3=Sen)" << std::endl;
        
        // Validación local 
        if (new_role < 1 || new_role > 3) {
            std::cerr << "\n" << client::colors::RED 
                      << "[!] Error: Role must be 1, 2, or 3." 
                      << client::colors::RESET << std::endl;
            return false;
        }

        try {
            // Hash password 
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_password);
            
            if (hashedPassword.empty()) {
                std::cerr << "\n" << client::colors::RED 
                          << "[!] Internal error processing password." 
                          << client::colors::RESET << std::endl;
                return false;
            }

            nlohmann::json payload = client::json_nlohmann::make_set_role_payload(approver_email, hashedPassword, target_email, new_role);

            nlohmann::json response = client::http::post_json_https("/user/change_level", payload);

            client::response_handler::handle_set_role_response(response);
            
            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                return true;
            }
            return false;

        } catch (const std::exception &e) {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error in operation: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }
}