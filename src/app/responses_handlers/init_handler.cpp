#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {
   void handle_init_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
          std::cout << "[!] Invalid server response." << std::endl;
          return;
      }

      std::string status = response["status"];

      if (status == "ok") {
          std::cout << "[+] Repository created successfully." << std::endl;
          
          if (response.contains("Repository_name")) {
              std::cout << "    Registered Name: " << response["Repository_name"] << std::endl;
          }
      } else {
          std::cout << "[-] Failed to create repository." << std::endl;
          
          if (response.contains("message")) { 
            std::cout << "    " << response["message"].get<std::string>() << std::endl;

          }
      }

      std::cout << "------------------------------------------" << std::endl;
   }
}