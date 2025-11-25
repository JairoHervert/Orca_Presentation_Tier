#include "client/response_handler.hpp"
#include <iostream>
// En client/response_handler.cpp

namespace client::response_handler {

    void handle_verify_response(const nlohmann::json &response) {
        std::cout << "\n--- Resultado de Verificacion de Usuario ---" << std::endl;
        
        // Validar formato básico
        if (!response.contains("status")) {
            std::cout << "[ERROR] Respuesta inválida del servidor." << std::endl;
            std::cout << "Raw: " << response.dump() << std::endl;
            return;
        }

        std::string status = response["status"].get<std::string>();

        if (status == "ok" || status == "success") {
            std::cout << "[EXITO] Usuario verificado correctamente." << std::endl;
            
            // Mostrar a quién verificamos si el dato viene
            if (response.contains("target_user_email")) {
                std::cout << "Usuario objetivo: " << response["target_user_email"].get<std::string>() << std::endl;
            }
        } else {
            std::cout << "[FALLO] No se pudo verificar al usuario." << std::endl;
            
            // Imprimir el motivo del error (sea del servidor o del parche de conexión)
            if (response.contains("message")) {
                std::cout  << response["message"].get<std::string>() << std::endl;
            } else {
                std::cout << "Motivo desconocido." << std::endl;
            }
        }
        
        std::cout << "--------------------------------------------" << std::endl;
    }
}