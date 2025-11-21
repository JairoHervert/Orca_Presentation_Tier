#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_revoke_response(const nlohmann::json &response) {
        std::cout << "\n--- Resultado de Revocacion ---" << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "success") {
                std::cout << "[EXITO] Permisos revocados correctamente." << std::endl;
            } else {
                std::cout << "[FALLO] No se pudieron revocar los permisos." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Detalle: " << response["message"] << std::endl;
        }
        
        std::cout << "-------------------------------" << std::endl;
    }

} 