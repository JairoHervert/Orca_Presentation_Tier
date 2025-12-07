#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_verify_response(const nlohmann::json &response) {
        std::cout<< std::endl;
        
        if (!response.contains("status")) {
            std::cout << "\n[-] Invalid server response." << std::endl;
            return;
        }

        std::string status = response["status"].get<std::string>();

        if (status == "ok" || status == "success") {
            std::cout << "[+] User verified successfully." << std::endl;
            
            // Mostrar a quién verificamos si el dato viene
            if (response.contains("target_user_email")) {
                std::cout << "    [*]Target user: " << response["target_user_email"].get<std::string>() << std::endl;
            }
        } else {
            std::cout << "\n[!] Could not verify user." << std::endl;
            
            // Imprimir el motivo del error si está disponible
            if (response.contains("message")) {
                std::cout  << "    " << response["message"].get<std::string>() << std::endl;
            } else {
                std::cout << "\n[!] Unknown reason." << std::endl;
            }
        }
        
        std::cout << "--------------------------------------------" << std::endl;
    }
}