#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_set_role_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                std::cout << " [+] User role updated successfully." << std::endl;
                if (response.contains("target_user_email") && response.contains("new_role")) {
                    std::cout << "    User: " << response["target_user_email"] 
                              << "\n     -> New Role: " << response["new_role"] << std::endl;
                }
            } else {
                std::cout << "\n[!] Failed to set role." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "    " << response["message"] << std::endl;
        }
        
        std::cout << "------------------------------------" << std::endl;
    }
}