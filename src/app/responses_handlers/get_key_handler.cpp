#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_get_key_response(const nlohmann::json &response) {
        std::cout << "\n--- Server Response (Get Key) ---" << std::endl;

        // 1. Validar integridad básica
        if (!response.contains("status")) {
            std::cerr << "[-] Error: Invalid server response (missing status)." << std::endl;
            return;
        }

        std::string status = response["status"];

        // 2. Manejar Éxito
        if (status == "ok" || status == "success") {
            std::cout << "[+] Encrypted key retrieved successfully." << std::endl;
            
            if (response.contains("project_alias")) {
                std::cout << "    Project: " << response["project_alias"].get<std::string>() << std::endl;
            }
            
            std::cout << "    Ready for local decryption..." << std::endl;

        } 
        // 3. Manejar Error
        else {
            std::cerr << "[-] Server rejected the request." << std::endl;
            if (response.contains("message")) { 
                std::cerr << "    Reason: " << response["message"] << std::endl;
            }
        }
        
        std::cout << "----------------------------------------" << std::endl;
    }

    // Agrego esta función auxiliar que usaste en run_get_key para extraer el dato
    std::string handle_get_key_response_data(const nlohmann::json &response) {
        handle_get_key_response(response); // Imprime los mensajes bonitos primero

        if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
            if (response.contains("encrypted_key")) {
                return response["encrypted_key"];
            }
        }
        return ""; // Retorna vacío si falló
    }

}