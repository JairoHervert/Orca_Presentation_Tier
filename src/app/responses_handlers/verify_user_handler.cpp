#include "client/colors.hpp" 
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_verify_user_response(const nlohmann::json &response) {
        std::cout<< std::endl;
        
        if (!response.contains("status")) {
            std::cout << "\n" << client::colors::RED 
                      << "[-] Invalid server response." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::string status = response["status"].get<std::string>();

        if (status == "ok" || status == "success") {
            std::cout << client::colors::GREEN 
                      << "[+] User verified successfully." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("target_user_email")) {
                std::cout << "    " << client::colors::YELLOW << "[*]Target user: " << client::colors::RESET 
                          << response["target_user_email"].get<std::string>() << std::endl;
            }
        } else {
            std::cout << "\n" << client::colors::RED 
                      << "[!] Could not verify user." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("message")) {
                std::cout << client::colors::RED 
                          << "    " << response["message"].get<std::string>() 
                          << client::colors::RESET << std::endl;
            } else {
                std::cout << client::colors::RED 
                          << "\n[!] Unknown reason." 
                          << client::colors::RESET << std::endl;
            }
        }
        
        std::cout << client::colors::MAGENTA 
                  << "--------------------------------------------" 
                  << client::colors::RESET << std::endl;
    }
}