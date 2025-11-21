#include "client/response_handler.hpp"

namespace client::response_handler {
    void handle_push_check_response(const nlohmann::json &response) {
        std::cout << "\n--- Respuesta del Servidor ---" << std::endl;
        if (response.contains("status")) {
            std::cout << "Estado: " << response["status"] << std::endl;
        }
        if (response.contains("server_hashes")) {
            std::cout << "Archivos remotos encontrados: " << response["server_hashes"].size() << std::endl;
        }
    }
}