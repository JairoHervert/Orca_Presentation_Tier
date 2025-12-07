#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_uncyp_response(const nlohmann::json &response) {
        std::cout << std::endl;

        if (!response.contains("status")) {
            std::cerr << "[!] Protocol error: Response without status." << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "ok") {
            std::cout << "[+] Request accepted by server." << std::endl;
            std::cout << "    Downloading encrypted packet..." << std::endl;
        } 
        
        else {
            std::cerr << "[-] Server rejected the request." << std::endl;
            
            if (response.contains("message")) {
                
                std::cerr << "    " <<  response["message"].get<std::string>() << std::endl;
            } else {
                std::cerr << "\n[!] Unknown reason." << std::endl;
            }
        }
    }
}