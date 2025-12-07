#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {

    void handle_change_status_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        if (response.contains("status")) {
            std::string statusResponse = response["status"];
            if (statusResponse == "ok" || statusResponse == "success") {
                std::cout << "[+] Estatus de usuario actualizado correctamente." << std::endl;
                
                if (response.contains("target_user_email")) {
                    std::cout << "Usuario: " << response["target_user_email"] << std::endl;
                }
                if (response.contains("new_status")) {
                    int statusCode = response["new_status"];
                    
                    std::string statusName;
                    switch(statusCode) {
                        case 1: statusName = "ACTIVO"; break;
                        case 0: statusName = "INACTIVO"; break;
                        case 2: statusName = "SUSPENDIDO"; break; 
                        default: statusName = "Estado Desconocido"; break;
                    }
                    
                    std::cout << "Nuevo Estatus: " << statusCode << " (" << statusName << ")" << std::endl;
                }
            } else {
                std::cout << "[-] No se pudo cambiar el estatus." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Detalle: " << response["message"] << std::endl;
        }
        
        std::cout << "----------------------------------" << std::endl;
    }
}