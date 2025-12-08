#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_encrypt_repo_response(const nlohmann::json &response) {
        std::cout << std::endl;
        
        if (response.contains("status")) {
            std::string status = response["status"];
            if (status == "ok" || status == "success") {
                std::cout << "[+] Repository encrypted successfully." << std::endl;
            } else {
                std::cout << "[-] Failed to encrypt repository." << std::endl;
            }
        }

        if (response.contains("message")) {
            std::cout << "    " << response["message"] << std::endl;
        }
        
        std::cout << "----------------------------------------------" << std::endl;
    }

}