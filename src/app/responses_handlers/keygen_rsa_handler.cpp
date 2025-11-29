#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {

   void handle_keygen_rsa_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
          std::cout << "[-] Error Respuesta invalida del servidor." << std::endl;
          return;
      }

      std::string status = response["status"];

      if (status == "ok" || status == "success") {
          std::cout << "[+] Clave de ENCRIPTACION (RSA) vinculada exitosamente." << std::endl;
          
          // El servidor suele devolver "key_saved": true
          if (response.contains("key_saved") && response["key_saved"].get<bool>()) {
              std::cout << "     Base de datos actualizada: SI" << std::endl;
          }
          
          if (response.contains("user_email")) {
              std::cout << "     Usuario: " << response["user_email"] << std::endl;
          }

      } else {
          std::cout << "[-] El servidor rechazo la clave RSA." << std::endl;
          
          if (response.contains("message")) {
              std::cout << "     Motivo: " << response["message"] << std::endl;
          } else {
              std::cout << "     Motivo desconocido." << std::endl;
          }
      }
      
      std::cout << "------------------------------------" << std::endl;
   }
}