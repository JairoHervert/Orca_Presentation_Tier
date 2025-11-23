#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_remove_response(const nlohmann::json &response) {
        std::cout << "\n--- Resultado de Eliminacion de Proyecto ---" << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "success") {
                std::cout << "Repositorio eliminado correctamente." << std::endl;
            } else {
                std::cout << "No se pudo eliminar el repositorio." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Detalle: " << response["message"] << std::endl;
        }
        
        std::cout << "--------------------------------------------" << std::endl;
    }

}