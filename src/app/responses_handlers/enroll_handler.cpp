#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_enroll_response(const nlohmann::json &response) {
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                std::cout << "[+] Usuario agregado al proyecto correctamente." << std::endl;
                
                if (response.contains("project_name"))
                    std::cout << "Proyecto: " << response["project_name"] << std::endl;
                
                if (response.contains("user_email"))
                    std::cout << "Usuario:  " << response["user_email"] << std::endl;
                    
            } else {
                std::cout << "[-] No se pudo agregar al usuario." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "Mensaje: " << response["message"] << std::endl;
        }
        
        std::cout << "--------------------------------------------" << std::endl;
    }

} 