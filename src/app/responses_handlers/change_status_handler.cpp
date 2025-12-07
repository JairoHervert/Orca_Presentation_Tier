#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_change_status_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        if (response.contains("status")) {
            std::string statusResponse = response["status"];
            if (statusResponse == "ok" || statusResponse == "success") {
                std::cout << "[+] User status updated successfully." << std::endl;
                
                if (response.contains("target_user_email")) {
                    std::cout << "    User: " << response["target_user_email"] << std::endl;
                }
                if (response.contains("new_status")) {
                    int statusCode = response["new_status"];
                    
                    std::string statusName;
                    switch(statusCode) {
                        case 1: statusName = "ACTIVE"; break;
                        case 0: statusName = "INACTIVE"; break;
                        default: statusName = "Unknown Status"; break;
                    }
                    
                    std::cout << "    New Status: " << statusCode << " (" << statusName << ")" << std::endl;
                }
            } else {
                std::cout << "\n[!] Failed to change status." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "    " << response["message"] << std::endl;
        }
        
        std::cout << "----------------------------------" << std::endl;
    }
}