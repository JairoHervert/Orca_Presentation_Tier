#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_list_repos_response(const nlohmann::json &response) {
        // Verificar si la respuesta contiene un error    
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n[-] Error: " << response.value("message", "Unknown error") << std::endl;
            return;
        }

        // Validar que exista la lista "projects" 
        if (!response.contains("projects") || !response["projects"].is_array()) {
            std::cout << "\n[!] No repositories found." << std::endl;
            return;
        }

        auto projects = response["projects"];

        if (projects.empty()) {
            std::cout << "\n[i] The repository list is empty." << std::endl;
            return;
        }

        std::cout << "\nAvailable Repositories:\n" << std::endl;
        // IMPRIMIR ENCABEZADOS
        std::cout << std::left 
                  << std::setw(20) << "NAME" 
                  << "DESCRIPTION" << std::endl;

        std::cout << std::string(80, '-') << std::endl;

        // IMPRIMIR FILAS
        for (const auto& proj : projects) {
            
            // Extraer datos 
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