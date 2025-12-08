#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_get_key_response(const nlohmann::json &response) {

        // Validar integridad básica
        if (!response.contains("status")) {
            std::cerr << "[-] Error: Invalid server response (missing status)." << std::endl;
            return;
        }

        std::string status = response["status"];

        // Manejar Éxito
        if (status == "ok" || status == "success") {
            if (response.contains("project_alias")) {
                std::cout << "\n[*] Project_enc: " << response["project_alias"].get<std::string>() << std::endl;
            }
            
            std::cout << "    Ready for local decryption..." << std::endl;

        } 
        // Manejar Error
        else {
            std::cerr << "[-] Server rejected the request." << std::endl;
            if (response.contains("message")) { 
                std::cerr << "    Reason: " << response["message"] << std::endl;
            }
        }
        
        std::cout << "----------------------------------------" << std::endl;
    }

    std::string handle_get_key_response_data(const nlohmann::json &response) {
        handle_get_key_response(response); 

        if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
            if (response.contains("encrypted_key")) {
                return response["encrypted_key"];
            }
        }
        return ""; 
    }

}