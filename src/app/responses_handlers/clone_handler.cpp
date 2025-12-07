#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {

    void handle_clone_response(const nlohmann::json &response) {
        
        if (!response.contains("status")) {
            std::cerr << "[!] Respuesta invalida del servidor (sin status)." << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            
            std::cout << "[+] Conexion establecida. Paquete de datos recibido." << std::endl;
            
        } else {
            
            std::cout << "[-] El servidor rechazo la solicitud de clonado." << std::endl;
            if (response.contains("message")) {
                std::cout <<  response["message"].get<std::string>() << std::endl;
            }
        }
    }
}