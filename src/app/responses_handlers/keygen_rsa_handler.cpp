#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {

   void handle_keygen_rsa_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
          std::cout << "[-] Error: Invalid server response." << std::endl;
          return;
      }

      std::string status = response["status"];

      if (status == "ok" || status == "success") {
        std::cout << "[+] Public key linked successfully." << std::endl;

          
        // El servidor suele devolver "key_saved": true
        if (response.contains("key_saved") && response["key_saved"].get<bool>()) {
            std::cout << "    Database updated: YES" << std::endl;
        }
          
        if (response.contains("user_email")) {
            std::cout << "    User: " << response["user_email"] << std::endl;
        }

       } else {
            std::cout << "[-] Server rejected the RSA key." << std::endl;
            
            if (response.contains("message")) {
                std::cout << "    " << response["message"] << std::endl;
            } else {
                std::cout << "    Unknown reason." << std::endl;
            }
      }     
      
      std::cout << "------------------------------------" << std::endl;
   }
}