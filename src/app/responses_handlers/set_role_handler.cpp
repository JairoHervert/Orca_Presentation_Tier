#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_set_role_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        bool success = false; 

        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                success = true;
                std::cout << client::colors::GREEN 
                          << " [+] User role updated successfully." 
                          << client::colors::RESET << std::endl;

                if (response.contains("target_user_email") && response.contains("new_role")) {
                    std::cout << "    " << client::colors::YELLOW << "User: " << client::colors::RESET 
                              << response["target_user_email"] 
                              << "\n    " << client::colors::YELLOW << " -> New Role: " << client::colors::RESET 
                              << response["new_role"] << std::endl;
                }
            } else {
                std::cout << "\n" << client::colors::RED 
                          << "[!] Failed to set role." 
                          << client::colors::RESET << std::endl;
            }
        }

        if (response.contains("message")) {
            if (success) {
                std::cout << "    " << response["message"] << std::endl;
            } else {
                std::cout << client::colors::RED 
                          << "    " << response["message"] 
                          << client::colors::RESET << std::endl;
            }
        }
        
        std::cout << client::colors::MAGENTA 
                  << "------------------------------------" 
                  << client::colors::RESET << std::endl;
    }
}