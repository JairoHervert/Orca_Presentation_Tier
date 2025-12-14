#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_log_response(const nlohmann::json &response) {
        
        // TITULO: MAGENTA + Bold
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "=== Historial del Proyecto (Commits) ===" 
                  << client::colors::RESET << std::endl;
        
        if (!response.contains("status") || response["status"] != "success") {
            std::cerr << client::colors::RED 
                      << "[-] No se pudo recuperar el historial." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("message")) 
                std::cerr << client::colors::RED << "    Detalle: " << response["message"] << client::colors::RESET << std::endl;
            return;
        }

        if (!response.contains("history") || response["history"].empty()) {
            // AVISO: Amarillo
            std::cout << "\n" << client::colors::YELLOW 
                      << "[i] No hay historial de cambios para este proyecto." 
                      << client::colors::RESET << std::endl;
            return;
        }

        auto history = response["history"];

        // Encabezado de la tabla (Negrita)
        std::cout << client::colors::BOLD << std::left 
                  << std::setw(25) << "Email" 
                  << std::setw(35) << "File"       
                  << std::setw(22) << "Date" 
                  << std::setw(15) << "Status" 
                  << client::colors::RESET << std::endl;

        std::cout << client::colors::MAGENTA << std::string(97, '-') << client::colors::RESET << std::endl;

        // Filas
        for (const auto& commit : history) {
            
            std::string status = commit.value("status", "Pending");
            
            // Lógica simple de color para el status
            const char* statusColor = client::colors::RESET;
            if (status == "success" || status == "ok" || status == "active") {
                statusColor = client::colors::GREEN;
            } else if (status == "error" || status == "deleted" || status == "failed") {
                statusColor = client::colors::RED;
            } else {
                statusColor = client::colors::YELLOW; // Pending u otros
            }

            std::cout << std::left 
                      // Email en Amarillo
                      << client::colors::YELLOW << std::setw(25) << commit.value("email", "N/A") << client::colors::RESET
                      // Archivo normal
                      << std::setw(35) << commit.value("file", "N/A")
                      // Fecha normal
                      << std::setw(22) << commit.value("date", "Unknown")
                      // Status coloreado
                      << statusColor << std::setw(15) << status << client::colors::RESET
                      << std::endl;
        }
        std::cout << client::colors::MAGENTA << std::string(97, '-') << client::colors::RESET << std::endl;
    }

}