#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {
   void handle_keygen_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
          std::cout << "[-] Error Respuesta del servidor no valida." << std::endl;
          std::cout << "Raw: " << response.dump() << std::endl;
          return;
      }

      std::string status = response["status"];

      if (status == "ok" || status == "success") {
          std::cout << "[+] Clave publica vinculada correctamente." << std::endl;
          
          if (response.contains("user_email")) {
              std::cout << "Usuario: " << response["user_email"] << std::endl;
          }
      } else {
          std::cout << "[-] El servidor no pudo registrar la clave." << std::endl;
          
          if (response.contains("message")) {
              std::cout << "Motivo: " << response["message"] << std::endl;
          }
      }
      
      std::cout << "---------------------------------------" << std::endl;
   }
}