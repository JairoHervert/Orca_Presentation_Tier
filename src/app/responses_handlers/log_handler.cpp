#include "client/response_handler.hpp"
#include <iomanip> 

namespace client::response_handler {

    void handle_log_response(const nlohmann::json &response) {
        std::cout << "\n=== Historial del Proyecto (Commits) ===" << std::endl;
        
        if (!response.contains("status") || response["status"] != "success") {
            std::cerr << "[ERROR] No se pudo recuperar el historial." << std::endl;
            if (response.contains("message")) std::cerr << "Detalle: " << response["message"] << std::endl;
            return;
        }

        if (!response.contains("history") || response["history"].empty()) {
            std::cout << "No hay historial de cambios para este proyecto." << std::endl;
            return;
        }

        auto history = response["history"];

        // Encabezado de la tabla 
        std::cout << std::left 
                  << std::setw(25) << "Email" 
                  << std::setw(35) << "File"       
                  << std::setw(22) << "Date" 
                  << std::setw(15) << "Status" 
                  << std::endl;
        std::cout << std::string(97, '-') << std::endl;

        // Filas
        for (const auto& commit : history) {
            std::cout << std::left 
                      << std::setw(25) << commit.value("email", "N/A")
                      << std::setw(35) << commit.value("file", "N/A")
                      << std::setw(22) << commit.value("date", "Unknown")
                      << std::setw(15) << commit.value("status", "Pending")
                      << std::endl;
        }
        std::cout << std::string(97, '-') << std::endl;
    }

} 