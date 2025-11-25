#include "client/response_handler.hpp"
#include <iostream>
#include <string>

namespace client::response_handler {

    void handle_nuser_response(const nlohmann::json &response) {
        
        // Validar que la respuesta tenga un status
        if (!response.contains("status")) {
            std::cerr << "[!] Error: La respuesta del servidor no tiene formato valido." << std::endl;
            std::cout << "Raw: " << response.dump() << std::endl; 
            return;
        }

        std::string status = response["status"].get<std::string>();

        // Caso de ÉXITO
        if (status == "ok") {
            std::cout << "[+] Usuario creado exitosamente." << std::endl;
            
            // Extraemos los datos que el servidor nos devolvió
            std::string name = response.value("user_name", "Desconocido");
            std::string email = response.value("user_email", "Sin email");

            std::cout << "    Nombre registrado: " << name << std::endl;
            std::cout << "    Email registrado:  " << email << std::endl;
        } 
        // Caso de ERROR
        else {
            std::cout << "[-] Fallo al crear usuario." << std::endl;
            
            if (response.contains("message")) {
                std::cout << response["message"].get<std::string>() << std::endl;
            } else {
                std::cout << "    Motivo desconocido." << std::endl;
            }
        }

        std::cout << "----------------------------------------------" << std::endl;
    }

}