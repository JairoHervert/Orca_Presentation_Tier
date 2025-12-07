#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {

    void handle_change_role_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                std::cout << " [+] Rol de usuario actualizado correctamente." << std::endl;
                if (response.contains("target_user_email") && response.contains("new_role")) {
                    std::cout << "Usuario: " << response["target_user_email"] 
                              << " -> Nuevo Rol: " << response["new_role"] << std::endl;
                }
            } else {
                std::cout << "[-] No se pudo cambiar el rol." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Detalle: " << response["message"] << std::endl;
        }
        
        std::cout << "------------------------------------" << std::endl;
    }
} 