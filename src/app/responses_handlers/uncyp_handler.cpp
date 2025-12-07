#include "client/response_handler.hpp"
#include <iostream>
#include <string>

namespace client::response_handler {

    void handle_uncyp_response(const nlohmann::json &response) {
        std::cout << std::endl;

        if (!response.contains("status")) {
            std::cerr << "[!] Error de protocolo: Respuesta sin status." << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            std::cout << "[+] Solicitud aceptada por el servidor." << std::endl;
            std::cout << "    Descargando paquete cifrado..." << std::endl;
        } 
        
        else {
            std::cerr << "[-] El servidor rechazo la solicitud." << std::endl;
            
            if (response.contains("message")) {
                
                std::cerr <<  response["message"].get<std::string>() << std::endl;
            } else {
                std::cerr << "Razon desconocida." << std::endl;
            }
        }
    }
}