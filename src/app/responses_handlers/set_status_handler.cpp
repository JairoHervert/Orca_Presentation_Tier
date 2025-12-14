#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_set_status_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        bool success = false; 

        if (response.contains("status")) {
            std::string statusResponse = response["status"];
            if (statusResponse == "ok" || statusResponse == "success") {
                success = true;
                std::cout << client::colors::GREEN 
                          << "[+] User status updated successfully." 
                          << client::colors::RESET << std::endl;
                
                if (response.contains("target_user_email")) {
                    std::cout << "    " << client::colors::YELLOW << "User: " << client::colors::RESET 
                              << response["target_user_email"] << std::endl;
                }
                if (response.contains("new_status")) {
                    int statusCode = response["new_status"];
                    
                    std::string statusName;
                    switch(statusCode) {
                        case 1: statusName = std::string(client::colors::GREEN) + "ACTIVE" + client::colors::RESET; break;
                        case 0: statusName = std::string(client::colors::RED) + "INACTIVE" + client::colors::RESET; break;
                        default: statusName = "Unknown Status"; break;
                    }
                    
                    std::cout << "    " << client::colors::YELLOW << "New Status: " << client::colors::RESET 
                              << statusCode << " (" << statusName << ")" << std::endl;
                }
            } else {
                std::cout << "\n" << client::colors::RED 
                          << "[!] Failed to set status." 
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
                  << "----------------------------------" 
                  << client::colors::RESET << std::endl;
    }
}