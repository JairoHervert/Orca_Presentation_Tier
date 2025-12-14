#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_list_repos_response(const nlohmann::json &response) {
        
        // Verificar si la respuesta contiene un error    
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error: " << response.value("message", "Unknown error") 
                      << client::colors::RESET << std::endl;
            return;
        }

        // Validar que exista la lista "projects" 
        if (!response.contains("projects") || !response["projects"].is_array()) {
            std::cout << "\n" << client::colors::RED 
                      << "[!] No repositories found." 
                      << client::colors::RESET << std::endl;
            return;
        }

        auto projects = response["projects"];

        if (projects.empty()) {
            std::cout << "\n" << client::colors::YELLOW 
                      << "[i] The repository list is empty." 
                      << client::colors::RESET << std::endl;
            return;
        }

        // TITULO: MAGENTA + Bold
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "Available Repositories:\n" 
                  << client::colors::RESET << std::endl;

        // IMPRIMIR ENCABEZADOS (Negrita)
        std::cout << client::colors::BOLD << std::left 
                  << std::setw(20) << "NAME" 
                  << "DESCRIPTION" 
                  << client::colors::RESET << std::endl;

        std::cout << client::colors::MAGENTA << std::string(80, '-') << client::colors::RESET << std::endl;

        // IMPRIMIR FILAS
        for (const auto& proj : projects) {
            
            // Extraer datos 
            std::string name = proj.value("name", "Unnamed");
            std::string desc = proj.value("description", "");

            if (desc.length() > 60) desc = desc.substr(0, 57) + "...";

            std::cout << std::left 
                      << client::colors::GREEN << std::setw(20) << name << client::colors::RESET
                      << desc << std::endl;
        }
        std::cout << std::endl;
    }
}