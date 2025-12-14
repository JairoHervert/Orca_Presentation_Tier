#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_get_key_response(const nlohmann::json &response) {

        // Validar integridad básica
        if (!response.contains("status")) {
            std::cerr << client::colors::RED 
                      << "[-] Error: Invalid server response (missing status)." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::string status = response["status"];

        // Manejar Éxito
        if (status == "ok" || status == "success") {
            if (response.contains("project_alias")) {
                std::cout << "\n" << client::colors::BLUE 
                          << "[*] Project_enc: " << client::colors::RESET 
                          << response["project_alias"].get<std::string>() << std::endl;
            }
            
            std::cout << client::colors::GREEN 
                      << "    Ready for local decryption..." 
                      << client::colors::RESET << std::endl;

        } 
        else {
            std::cerr << client::colors::RED 
                      << "[-] Server rejected the request." 
                      << client::colors::RESET << std::endl;
            if (response.contains("message")) { 
                std::cerr << client::colors::RED 
                          << "    Reason: " << response["message"] 
                          << client::colors::RESET << std::endl;
            }
        }
        
        std::cout << client::colors::MAGENTA 
                  << "----------------------------------------" 
                  << client::colors::RESET << std::endl;
    }

    std::string handle_get_key_response_data(const nlohmann::json &response) {
        handle_get_key_response(response); 

        if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
            // Nota: Aquí buscamos 'encrypted_key' o 'aes_rsa_key' según tu protocolo. 
            // En 'get_key.cpp' buscabas "aes_rsa_key", asegúrate que coincida.
            if (response.contains("aes_rsa_key")) {
                 return response["aes_rsa_key"];
            }

            if (response.contains("encrypted_key")) {
                return response["encrypted_key"];
            }
        }
        return ""; 
    }

}