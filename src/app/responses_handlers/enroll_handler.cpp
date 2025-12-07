#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_enroll_response(const nlohmann::json &response) {
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                std::cout << "\n[+] User added to project successfully." << std::endl;
                
                if (response.contains("project_name"))
                    std::cout << "    Project: " << response["project_name"] << std::endl;
                
                if (response.contains("user_email"))
                    std::cout << "    User:    " << response["user_email"] << std::endl;
                    
            } else {
                std::cout << "\n[!] Failed to add user." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "    " << response["message"] << std::endl;
        }
        
        std::cout << "--------------------------------------------" << std::endl;
    }

}