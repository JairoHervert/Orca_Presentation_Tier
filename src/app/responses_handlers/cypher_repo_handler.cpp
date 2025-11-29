#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {

    void handle_cypher_repo_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                std::cout << "[+] El repositorio ha sido cifrado correctamente." << std::endl;
            } else {
                std::cout << "[-] No se pudo cifrar el repositorio." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Mensaje: " << response["message"] << std::endl;
        }
        
        std::cout << "----------------------------------------------" << std::endl;
    }

} 
