#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_grant_access_file_response(const nlohmann::json &response) {
        
        if (!response.contains("status")) {
            std::cerr << "\n" << client::colors::RED 
                      << "[!] Error: Invalid server response." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            std::cout << "\n" << client::colors::GREEN 
                      << "[+] Permission granted successfully." 
                      << client::colors::RESET << std::endl;
            
            // Mostramos detalles si vienen en la respuesta (Etiquetas en Amarillo)
            if (response.contains("target_user") && response.contains("file_name")) {
                std::cout << "    " << client::colors::YELLOW << "User: " << client::colors::RESET 
                          << response["target_user"].get<std::string>() << std::endl;
                std::cout << "    " << client::colors::YELLOW << "File: " << client::colors::RESET 
                          << response["file_name"].get<std::string>() << std::endl;
            }
            if (response.contains("project_name")) {
                 std::cout << "    " << client::colors::YELLOW << "Project: " << client::colors::RESET 
                           << response["project_name"].get<std::string>() << std::endl;
            }

        } else {
            std::cout << "\n" << client::colors::RED 
                      << "[-] Failed to add user to file." 
                      << client::colors::RESET << std::endl;
            if (response.contains("message")) {
                std::cout << client::colors::RED 
                          << "    " << response["message"].get<std::string>() 
                          << client::colors::RESET << std::endl;
            }
        }
        std::cout << client::colors::MAGENTA 
                  << "----------------------------------------------" 
                  << client::colors::RESET << std::endl;
    }
}