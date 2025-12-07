#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {

    void handle_clone_response(const nlohmann::json &response) {
        
        if (!response.contains("status")) {
            std::cerr << "[!] Invalid server response (missing status)." << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            
            std::cout << "\n[+] Connection established. Data packet received." << std::endl;
            
        } else {
            
            std::cout << "\n[-] The server rejected the clone request." << std::endl;
            if (response.contains("message")) {
                std::cout << "    " << response["message"].get<std::string>() << std::endl;
            }
        }
    }
}