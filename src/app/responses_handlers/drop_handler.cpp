#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_drop_response(const nlohmann::json &response) {
        std::cout << "\n--- Resultado de Baja de Usuario ---" << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "success") {
                std::cout << " Usuario dado de baja correctamente." << std::endl;
            } else {
                std::cout << "No se pudo dar de baja al usuario." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Detalle: " << response["message"] << std::endl;
        }
        
        std::cout << "------------------------------------" << std::endl;
    }

}