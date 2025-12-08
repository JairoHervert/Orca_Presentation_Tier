#include "client/response_handler.hpp"
namespace client::response_handler {

    void handle_push_check_response(const nlohmann::json &response) {
        if (!response.contains("status")) return;

        if (response["status"] != "success" && response["status"] != "ok") {
            std::cerr << "\n[!] Server reported an issue checking status." << std::endl;
            if (response.contains("message")) {
                std::cerr << "    " << response["message"] << std::endl;
            }
        } else {
            std::cout << "\n[+] Remote status synchronized." << std::endl;
        }
    }

    void handle_push_upload_response(const nlohmann::json &response) {
        std::cout << "\n--- Synchronization Result (Push) ---" << std::endl;

        // Validar integridad
        if (!response.contains("status")) {
            std::cerr << "[!] Invalid server response (Missing status)." << std::endl;
            std::cout << "    " << response.dump() << std::endl;
            return;
        }

        std::string status = response["status"];

        if (status == "success" || status == "ok") {
            std::cout << "[+] Repository updated successfully." << std::endl;
            
            // Detalles extra (Si tu servidor los manda, como en server_test.cpp)
            if (response.contains("repoName")) {
                std::cout << "    Project: " << response["repoName"].get<std::string>() << std::endl;
            }
            
            if (response.contains("filesReceived")) {
                std::cout << "    Files processed: " << response["filesReceived"] << std::endl;
            }

            if (response.contains("message")) {
                std::cout << "    " << response["message"].get<std::string>() << std::endl;
            }

        } else {
            std::cout << "[-] Upload rejected." << std::endl;
            
            if (response.contains("message")) {
                std::cout << "    " << response["message"].get<std::string>() << std::endl;
            }
        }

        std::cout << "------------------------------------------" << std::endl;
    }

}