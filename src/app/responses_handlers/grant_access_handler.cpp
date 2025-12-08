#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_grant_access_file_response(const nlohmann::json &response) {
        
        if (!response.contains("status")) {
            std::cerr << "\n[!] Error: Invalid server response." << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            std::cout << "\n[+] Permission granted successfully." << std::endl;
            
            // Mostramos detalles si vienen en la respuesta
            if (response.contains("target_user") && response.contains("file_name")) {
                std::cout << "    User: " << response["target_user"].get<std::string>() << std::endl;
                std::cout << "    File: " << response["file_name"].get<std::string>() << std::endl;
            }
            if (response.contains("project_name")) {
                 std::cout << "    Project: " << response["project_name"].get<std::string>() << std::endl;
            }

        } else {
            std::cout << "\n[-] Failed to add user to file." << std::endl;
            if (response.contains("message")) {
                std::cout << "    " << response["message"].get<std::string>() << std::endl;
            }
        }
        std::cout << "----------------------------------------------" << std::endl;
    }
}