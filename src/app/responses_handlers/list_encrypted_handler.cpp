#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_list_encrypted_response(const nlohmann::json &response) {
        
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n[-] Error: " << response.value("message", "Unknown or auth failed") << std::endl;
            return;
        }

        if (!response.contains("projects") || !response["projects"].is_array()) {
            std::cout << "\n[!] No encrypted repositories found." << std::endl;
            return;
        }

        auto projects = response["projects"];

        if (projects.empty()) {
            std::cout << "\n[i] You don't have access to any encrypted repository." << std::endl;
            return;
        }

        std::cout << "\nEncrypted Repositories:\n" << std::endl;

        // TABLA: NAME | DESCRIPTION
        std::cout << std::left 
                  << std::setw(20) << "NAME" 
                  << "DESCRIPTION" << std::endl;

        std::cout << std::string(80, '-') << std::endl; 

        for (const auto& proj : projects) {
            
            std::string name = proj.value("name", "Unnamed");
            std::string desc = proj.value("description", "");

            if (desc.length() > 60) desc = desc.substr(0, 57) + "...";

            std::cout << std::left 
                      << std::setw(20) << name 
                      << desc << std::endl;
        }
        std::cout << std::endl;
    }
}