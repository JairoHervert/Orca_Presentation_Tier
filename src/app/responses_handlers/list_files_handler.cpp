#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_list_files_response(const nlohmann::json &response, const std::string& repo_name) {
        
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error: " << response.value("message", "Unknown or access denied") 
                      << client::colors::RESET << std::endl;
            return;
        }

        if (!response.contains("files") || !response["files"].is_array()) {
            std::cout << "\n" << client::colors::RED 
                      << "[!] No files found or incorrect format." 
                      << client::colors::RESET << std::endl;
            return;
        }

        auto files = response["files"];

        if (files.empty()) {
            std::cout << "\n" << client::colors::YELLOW 
                      << "[i] No accessible files found in repository '" << repo_name << "'." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::cout << std::endl;

        std::string header = "REPO: " + repo_name;
        
        int width = std::max((int)header.length(), 40); 

        // TITULO: MAGENTA + Bold
        std::cout << client::colors::BOLD << client::colors::MAGENTA 
                  << std::left << std::setw(width) << header 
                  << client::colors::RESET << std::endl;

        std::cout << client::colors::MAGENTA << std::string(width, '-') << client::colors::RESET << std::endl; 

        for (const auto& file_obj : files) {
            std::string route = file_obj.value("route", "Unknown File");

            std::cout << client::colors::GREEN << route << client::colors::RESET << std::endl;
        }
        std::cout << std::endl;
    }
}