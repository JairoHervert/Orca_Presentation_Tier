#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_list_files_response(const nlohmann::json &response, const std::string& repo_name) {
        
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n[-] Error: " << response.value("message", "Unknown or access denied") << std::endl;
            return;
        }

        if (!response.contains("files") || !response["files"].is_array()) {
            std::cout << "\n[!] No files found or incorrect format." << std::endl;
            return;
        }

        auto files = response["files"];

        if (files.empty()) {
            std::cout << "\n[i] No accessible files found in repository '" << repo_name << "'." << std::endl;
            return;
        }

        std::cout << std::endl;

        std::string header = "REPO: " + repo_name;
        
        int width = std::max((int)header.length(), 40); 

        std::cout << std::left << std::setw(width) << header << std::endl;
        std::cout << std::string(width, '-') << std::endl; 

        for (const auto& file_obj : files) {
            std::string route = file_obj.value("route", "Unknown File");

            std::cout << route << std::endl;
        }
        std::cout << std::endl;
    }
}