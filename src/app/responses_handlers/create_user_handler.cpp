#include "client/colors.hpp" 
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_create_user_response(const nlohmann::json &response) {
        
        // Validar que la respuesta tenga un status
        if (!response.contains("status")) {
            std::cerr << client::colors::RED 
                      << "[!] Error: Server response has invalid format." 
                      << client::colors::RESET << std::endl;
            std::cout << "Raw: " << response.dump() << std::endl; 
            return;
        }

        std::string status = response["status"].get<std::string>();

        if (status == "ok") {
            std::cout << "\n" << client::colors::GREEN 
                      << "[+] User created successfully." 
                      << client::colors::RESET << std::endl;
            
            std::string name = response.value("user_name", "Unknown");
            std::string email = response.value("user_email", "No email");

            std::cout << "    " << client::colors::YELLOW << "Registered Name:  " << client::colors::RESET << name << std::endl;
            std::cout << "    " << client::colors::YELLOW << "Registered Email: " << client::colors::RESET << email << std::endl;
        } 
        else {
            std::cout << "\n" << client::colors::RED 
                      << "[-] Failed to create user." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("message")) {
                std::cout << client::colors::RED 
                          << "    " << response["message"].get<std::string>() 
                          << client::colors::RESET << std::endl;
            } else {
                std::cout << "    Unknown reason." << std::endl;
            }
        }

        std::cout << client::colors::MAGENTA << "----------------------------------------------" << client::colors::RESET << std::endl;
    }

}