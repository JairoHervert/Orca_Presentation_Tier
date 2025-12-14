#include "client/colors.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {
   void handle_keygen_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
          std::cerr << client::colors::RED 
                    << "[-] Error: Invalid server response." 
                    << client::colors::RESET << std::endl;
          return;
      }

      std::string status = response["status"];

      if (status == "ok" || status == "success") {
          std::cout << client::colors::GREEN 
                    << "[+] Public key linked successfully." 
                    << client::colors::RESET << std::endl;
          
          if (response.contains("user_email")) {
              std::cout << "    " << client::colors::YELLOW << "User: " << client::colors::RESET 
                        << response["user_email"] << std::endl;
          }
      } else {
          std::cout << client::colors::RED 
                    << "[-] Server failed to register the key." 
                    << client::colors::RESET << std::endl;
          
          if (response.contains("message")) {
              std::cout << client::colors::RED 
                        << "    " << response["message"] 
                        << client::colors::RESET << std::endl;
          }
      }
      
      std::cout << client::colors::MAGENTA 
                << "---------------------------------------" 
                << client::colors::RESET << std::endl;
   }
}