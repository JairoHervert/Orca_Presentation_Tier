#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_config_response(const nlohmann::json &response) {
        std::cout << "\n--- Respuesta del Servidor (Config) ---" << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "success") {
                std::cout << "Configuracion guardada exitosamente." << std::endl;
            } else {
                std::cout << "No se pudo guardar la configuracion." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Servidor dice: " << response["message"] << std::endl;
        }
        std::cout << "---------------------------------------" << std::endl;
    }

}