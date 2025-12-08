#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_create_user_response(const nlohmann::json &response) {
        
        // Validar que la respuesta tenga un status
        if (!response.contains("status")) {
            std::cerr << "[!] Error: Server response has invalid format." << std::endl;
            std::cout << "Raw: " << response.dump() << std::endl; 
            return;
        }

        std::string status = response["status"].get<std::string>();

        if (status == "ok") {
            std::cout << "[+] User created successfully." << std::endl;
            
            // Extraemos los datos que el servidor nos devolvió
            std::string name = response.value("user_name", "Unknown");
            std::string email = response.value("user_email", "No email");

            std::cout << "    Registered Name: " << name << std::endl;
            std::cout << "    Registered Email:  " << email << std::endl;
        } 
        // Caso de ERROR
        else {
            std::cout << "[-] Failed to create user." << std::endl;
            
            if (response.contains("message")) {
                std::cout << "    " << response["message"].get<std::string>() << std::endl;
            } else {
                std::cout << "    Unknown reason." << std::endl;
            }
        }

        std::cout << "----------------------------------------------" << std::endl;
    }

}