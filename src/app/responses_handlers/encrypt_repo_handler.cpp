#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_encrypt_repo_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        bool is_success = false;

        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                is_success = true;
                std::cout << client::colors::GREEN 
                          << "[+] Repository encrypted successfully." 
                          << client::colors::RESET << std::endl;
            } else {
                std::cout << client::colors::RED 
                          << "[-] Failed to encrypt repository." 
                          << client::colors::RESET << std::endl;
            }
        }

        if (response.contains("message")) {
            if (is_success) {
                std::cout << "    " << response["message"] << std::endl;
            } else {
                std::cout << client::colors::RED 
                          << "    " << response["message"] 
                          << client::colors::RESET << std::endl;
            }
        }
        
        std::cout << client::colors::MAGENTA 
                  << "----------------------------------------------" 
                  << client::colors::RESET << std::endl;
    }

}