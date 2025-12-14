#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp"
#include "client/response_handler.hpp"


namespace client::cmd {

    bool run_enroll_user(const std::string& approver_email, const std::string& approver_password, const std::string& project_name, const std::string& target_email) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << " --- Adding Collaborator to Project ---" 
                  << client::colors::RESET << std::endl;
        
        std::cout << client::colors::YELLOW << "Project:   " << client::colors::RESET << project_name << std::endl;
        std::cout << client::colors::YELLOW << "Approver:  " << client::colors::RESET << approver_email << std::endl;
        std::cout << client::colors::YELLOW << "New User:  " << client::colors::RESET << target_email << std::endl;

        try {
            // Hashear password del aprobador
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_password);
            if (hashedPassword.empty()) {
                std::cerr << "\n" << client::colors::RED 
                          << "[!] Internal error processing password." 
                          << client::colors::RESET << std::endl;
                return false;
            }

            // Crear Payload
            nlohmann::json payload = client::json_nlohmann::make_enroll_user_payload(approver_email, hashedPassword, project_name, target_email);

            nlohmann::json response = client::http::post_json_https("/repo/add_user", payload);

            client::response_handler::handle_enroll_user_response(response);

            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                return true;
            }
            return false;

        } catch (const std::exception &e) {
            std::cerr << client::colors::RED 
                      << "[-] Error in enroll_user: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }

}