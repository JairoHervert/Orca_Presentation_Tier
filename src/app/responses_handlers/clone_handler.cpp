#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_clone_response(const nlohmann::json &response) {
        
        if (!response.contains("status")) {
            std::cerr << client::colors::RED 
                      << "[!] Invalid server response (missing status)." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            std::cout << "\n" << client::colors::GREEN 
                      << "[+] Connection established. Data packet received." 
                      << client::colors::RESET << std::endl;
            
        } else {
            std::cout << "\n" << client::colors::RED 
                      << "[-] The server rejected the clone request." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("message")) {
                std::cout << client::colors::RED 
                          << "    " << response["message"].get<std::string>() 
                          << client::colors::RESET << std::endl;
            }
        }
    }
}