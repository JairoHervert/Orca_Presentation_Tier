#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_enroll_user_response(const nlohmann::json &response) {
        
        bool success = false; 

        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                success = true;
                std::cout << "\n" << client::colors::GREEN 
                          << "[+] User added to project successfully." 
                          << client::colors::RESET << std::endl;
                
                if (response.contains("project_name"))
                    std::cout << "    " << client::colors::YELLOW << "Project: " << client::colors::RESET 
                              << response["project_name"] << std::endl;
                
                if (response.contains("user_email"))
                    std::cout << "    " << client::colors::YELLOW << "User:    " << client::colors::RESET 
                              << response["user_email"] << std::endl;
                    
            } else {
                std::cout << "\n" << client::colors::RED 
                          << "[!] Failed to add user." 
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
                  << "--------------------------------------------" 
                  << client::colors::RESET << std::endl;
    }

}