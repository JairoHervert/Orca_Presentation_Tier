#include "client/response_handler.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm> // Necessary for std::reverse

namespace client::response_handler {

    void handle_commits_response(const nlohmann::json &response) {
        
        // 1. Error validations
        if (response.contains("status") && response["status"] == "error") {
            std::cerr << "\n[-] Error fetching commits: " << response.value("message", "Unknown") << std::endl;
            return;
        }

        if (!response.contains("commits") || !response["commits"].is_array()) {
            std::cout << "\n[!] No commits found or incorrect format." << std::endl;
            return;
        }

        // Copy the commits array to manipulate it
        auto commits = response["commits"];

        if (commits.empty()) {
            std::cout << "\n[i] Commit history is empty." << std::endl;
            return;
        }

        // 2. REVERSE ORDER (To see the most recent ones at the top)
        std::reverse(commits.begin(), commits.end());

        std::cout << "\nCommit History:\n" << std::endl;

        // 3. HEADERS (Only the 4 requested columns)
        // Adjust widths (setw) for a clean look
        std::cout << std::left 
                  << std::setw(25) << "COMMAND" 
                  << std::setw(22) << "DATE" 
                  << std::setw(10) << "STATUS" 
                  << "DESCRIPTION" << std::endl;

        std::cout << std::string(100, '-') << std::endl; // Divider line

        // 4. PRINT ROWS
        for (const auto& commit : commits) {
            
            // Extract data (using default values if missing)
            std::string cmd  = commit.value("command", "UNKNOWN");
            std::string date = commit.value("date", "N/A");
            bool accepted    = commit.value("accepted", false);
            std::string desc = commit.value("description", "");

            if (desc.length() > 50) desc = desc.substr(0, 47) + "...";

            std::cout << std::left 
                      << std::setw(25) << cmd 
                      << std::setw(22) << date 
                      << std::setw(10) << (accepted ? "OK" : "ERROR")
                      << desc << std::endl;
        }
        std::cout << std::endl;
    }
}