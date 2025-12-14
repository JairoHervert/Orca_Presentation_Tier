#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {

   void handle_init_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
         std::cerr << client::colors::RED 
                   << "[!] Invalid server response." 
                   << client::colors::RESET << std::endl;
         return;
      }

      std::string status = response["status"];

      if (status == "ok") {
         std::cout << client::colors::GREEN 
                   << "[+] Repository created successfully." 
                   << client::colors::RESET << std::endl;
         
         if (response.contains("Repository_name")) {
             std::cout << "    " << client::colors::YELLOW << "Registered Name: " << client::colors::RESET 
                       << response["Repository_name"] << std::endl;
         }
      } else {
         std::cout << client::colors::RED 
                   << "[-] Failed to create repository." 
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