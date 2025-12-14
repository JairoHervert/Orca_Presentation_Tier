#include "client/colors.hpp"
#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_list_encrypted_response(const nlohmann::json &response) {
        
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error: " << response.value("message", "Unknown or auth failed") 
                      << client::colors::RESET << std::endl;
            return;
        }

        if (!response.contains("projects") || !response["projects"].is_array()) {
            std::cout << "\n" << client::colors::RED 
                      << "[!] No encrypted repositories found." 
                      << client::colors::RESET << std::endl;
            return;
        }

        auto projects = response["projects"];

        if (projects.empty()) {
            std::cout << "\n" << client::colors::YELLOW 
                      << "[i] You don't have access to any encrypted repository." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "Encrypted Repositories:\n" 
                  << client::colors::RESET << std::endl;

        // TABLA: Encabezados en Negrita
        std::cout << client::colors::BOLD << std::left 
                  << std::setw(20) << "NAME" 
                  << "DESCRIPTION" 
                  << client::colors::RESET << std::endl;

        std::cout << client::colors::MAGENTA << std::string(80, '-') << client::colors::RESET << std::endl; 

        for (const auto& proj : projects) {
            
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