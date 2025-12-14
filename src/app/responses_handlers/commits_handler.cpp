#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

    void handle_commits_response(const nlohmann::json &response) {
        
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error fetching commits: " << response.value("message", "Unknown") 
                      << client::colors::RESET << std::endl;
            return;
        }

        if (!response.contains("commits") || !response["commits"].is_array()) {
            std::cout << "\n" << client::colors::RED 
                      << "[!] No commits found or incorrect format." 
                      << client::colors::RESET << std::endl;
            return;
        }

        auto commits = response["commits"];

        if (commits.empty()) {
            std::cout << "\n" << client::colors::YELLOW 
                      << "[i] Commit history is empty." 
                      << client::colors::RESET << std::endl;
            return;
        }

        std::reverse(commits.begin(), commits.end());

        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "Commit History:\n" 
                  << client::colors::RESET << std::endl;

        // Encabezados de tabla en Negrita para distinguir
        std::cout << client::colors::BOLD << std::left 
                  << std::setw(26) << "COMMAND" 
                  << std::setw(21) << "DATE" 
                  << std::setw(8)  << "STATUS" 
                  << "DESCRIPTION" 
                  << client::colors::RESET << std::endl;

        std::cout << std::string(150, '-') << std::endl; 

        for (const auto& commit : commits) {
            
            std::string cmd  = commit.value("command", "UNKNOWN");
            std::string date = commit.value("date", "N/A");
            bool accepted    = commit.value("accepted", false);
            std::string desc = commit.value("description", "");

            if (desc.length() > 100) desc = desc.substr(0, 97) + "...";

            const char* statusColor = accepted ? client::colors::GREEN : client::colors::RED;
            std::string statusText  = accepted ? "OK" : "FAILED";

            std::cout << std::left 
                      << client::colors::YELLOW << std::setw(26) << cmd << client::colors::RESET
                      << std::setw(21) << date 
                      << statusColor << std::setw(8)  << statusText << client::colors::RESET
                      << desc << std::endl;
        }
        std::cout << std::endl;
    }
}