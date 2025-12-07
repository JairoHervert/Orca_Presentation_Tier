#include "client/response_handler.hpp"
namespace client::response_handler {
   void handle_keygen_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
          std::cout << "[-] Error: Invalid server response." << std::endl;
          return;
      }

      std::string status = response["status"];

      if (status == "ok" || status == "success") {
          std::cout << "[+] Public key linked successfully." << std::endl;
          
          if (response.contains("user_email")) {
              std::cout << "    User: " << response["user_email"] << std::endl;
          }
      } else {
          std::cout << "[-] Server failed to register the key." << std::endl;
          
          if (response.contains("message")) {
              std::cout << "    " << response["message"] << std::endl;
          }
      }
      
      std::cout << "---------------------------------------" << std::endl;
   }
}