#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_decrypt_repo_response(const nlohmann::json &response) {
        std::cout << std::endl;

        if (!response.contains("status")) {
            std::cerr << client::colors::RED 
                      << "[!] Protocol error: Response without status." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            std::cout << client::colors::GREEN 
                      << "[+] Request accepted by server." 
                      << client::colors::RESET << std::endl;
            
            std::cout << "    " << client::colors::BLUE 
                      << "Downloading encrypted packet..." 
                      << client::colors::RESET << std::endl;
        } 
        else {
            std::cerr << client::colors::RED 
                      << "[-] Server rejected the request." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("message")) {
                std::cerr << client::colors::RED 
                          << "    " <<  response["message"].get<std::string>() 
                          << client::colors::RESET << std::endl;
            } else {
                std::cerr << client::colors::RED 
                          << "    [!] Unknown reason." 
                          << client::colors::RESET << std::endl;
            }
        }
    }
}