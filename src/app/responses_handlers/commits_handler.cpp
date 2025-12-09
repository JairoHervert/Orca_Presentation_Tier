#include "client/response_handler.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm> 

namespace client::response_handler {

    void handle_commits_response(const nlohmann::json &response) {
        
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n[-] Error fetching commits: " << response.value("message", "Unknown") << std::endl;
            return;
        }

        if (!response.contains("commits") || !response["commits"].is_array()) {
            std::cout << "\n[!] No commits found or incorrect format." << std::endl;
            return;
        }

        auto commits = response["commits"];

        if (commits.empty()) {
            std::cout << "\n[i] Commit history is empty." << std::endl;
            return;
        }

        std::reverse(commits.begin(), commits.end());

        std::cout << "\nCommit History:\n" << std::endl;

        std::cout << std::left 
                  << std::setw(26) << "COMMAND" 
                  << std::setw(21) << "DATE" 
                  << std::setw(8)  << "STATUS" 
                  << "DESCRIPTION" << std::endl;

        std::cout << std::string(150, '-') << std::endl; 

        for (const auto& commit : commits) {
            
            std::string cmd  = commit.value("command", "UNKNOWN");
            std::string date = commit.value("date", "N/A");
            bool accepted    = commit.value("accepted", false);
            std::string desc = commit.value("description", "");

            if (desc.length() > 100) desc = desc.substr(0, 97) + "...";

            std::cout << std::left 
                      << std::setw(26) << cmd 
                      << std::setw(21) << date 
                      << std::setw(8)  << (accepted ? "OK" : "FAILED") 
                      << desc << std::endl;
        }
        std::cout << std::endl;
    }
}