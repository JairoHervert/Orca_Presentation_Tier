#include "client/response_handler.hpp"
#include <iostream>
#include <iomanip>

namespace client::response_handler {

    void handle_list_accessible_response(const nlohmann::json &response) {
        
        // 1. Chequeo de errores
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n[-] Error: " << response.value("message", "Unknown or auth failed") << std::endl;
            return;
        }

        // 2. Validar lista "projects"
        if (!response.contains("projects") || !response["projects"].is_array()) {
            std::cout << "\n[!] No accessible repositories found." << std::endl;
            return;
        }

        auto projects = response["projects"];

        if (projects.empty()) {
            std::cout << "\n[i] You are not associated with any repository." << std::endl;
            return;
        }

        std::cout << "\nMy Repositories:\n" << std::endl;

        // 3. TABLA
        std::cout << std::left 
                  << std::setw(20) << "NAME" 
                  << "DESCRIPTION" << std::endl;

        std::cout << std::string(80, '-') << std::endl; 

        for (const auto& proj : projects) {
            
            std::string name = proj.value("name", "Unnamed");
            std::string desc = proj.value("description", "");

            // Cortar descripción si es muy larga
            if (desc.length() > 60) desc = desc.substr(0, 57) + "...";

            std::cout << std::left 
                      << std::setw(20) << name 
                      << desc << std::endl;
        }
        std::cout << std::endl;
    }
}