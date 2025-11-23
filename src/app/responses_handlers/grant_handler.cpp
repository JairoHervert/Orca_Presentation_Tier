#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_grant_response(const nlohmann::json &response) {
        std::cout << "\n--- Resultado de Asignacion (Grant) ---" << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "success") {
                std::cout << "Permisos concedidos correctamente." << std::endl;
            } else {
                std::cout << "No se pudieron conceder los permisos." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Detalle: " << response["message"] << std::endl;
        }
        
        std::cout << "---------------------------------------" << std::endl;
    }

} 