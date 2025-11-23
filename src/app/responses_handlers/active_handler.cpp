#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_active_response(const nlohmann::json &response) {
        std::cout << "\n--- Resultado de Activacion de Usuario ---" << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "success") {
                std::cout << " Usuario activado correctamente." << std::endl;
            } else {
                std::cout << "[FALLO] No se pudo activar al usuario." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Detalle: " << response["message"] << std::endl;
        }
        
        std::cout << "------------------------------------------" << std::endl;
    }

} 