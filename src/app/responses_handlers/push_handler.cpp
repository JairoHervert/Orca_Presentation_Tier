#include "client/colors.hpp" 
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_push_check_response(const nlohmann::json &response) {
        if (!response.contains("status")) return;

        if (response["status"] != "success" && response["status"] != "ok") {
            std::cerr << "\n" << client::colors::RED 
                      << "[!] Server reported an issue checking status." 
                      << client::colors::RESET << std::endl;
            if (response.contains("message")) {
                std::cerr << client::colors::RED 
                          << "    " << response["message"] 
                          << client::colors::RESET << std::endl;
            }
        } else {
            std::cout << "\n" << client::colors::GREEN 
                      << "[+] Remote status synchronized." 
                      << client::colors::RESET << std::endl;
        }
    }

    void handle_push_upload_response(const nlohmann::json &response) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "--- Synchronization Result (Push) ---" 
                  << client::colors::RESET << std::endl;

        // Validar integridad
        if (!response.contains("status")) {
            std::cerr << client::colors::RED 
                      << "[!] Invalid server response (Missing status)." 
                      << client::colors::RESET << std::endl;
            std::cout << "    " << response.dump() << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "success" || status == "ok") {
            std::cout << client::colors::GREEN 
                      << "[+] Repository updated successfully." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("repoName")) {
                std::cout << "    " << client::colors::YELLOW << "Project: " << client::colors::RESET 
                          << response["repoName"].get<std::string>() << std::endl;
            }
            
            if (response.contains("filesReceived")) {
                std::cout << "    " << client::colors::YELLOW << "Files processed: " << client::colors::RESET 
                          << response["filesReceived"] << std::endl;
            }

            if (response.contains("message")) {
                std::cout << "    " << response["message"].get<std::string>() << std::endl;
            }

        } else {
            std::cout << client::colors::RED 
                      << "[-] Upload rejected." 
                      << client::colors::RESET << std::endl;
            
            if (response.contains("message")) {
                std::cout << client::colors::RED 
                          << "    " << response["message"].get<std::string>() 
                          << client::colors::RESET << std::endl;
            }
        }

        std::cout << client::colors::MAGENTA 
                  << "------------------------------------------" 
                  << client::colors::RESET << std::endl;
    }

}