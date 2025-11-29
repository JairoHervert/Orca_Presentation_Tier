#include "client/response_handler.hpp"
#include <iostream>

namespace client::response_handler {
   void handle_init_response(const nlohmann::json &response) {

      if (!response.contains("status")) {
          std::cout << "[!] Respuesta invalida del servidor." << std::endl;
          return;
      }

      std::string status = response["status"];

      if (status == "ok") {
          std::cout << "[+] Repositorio creado correctamente." << std::endl;
          
          if (response.contains("Repository_name")) {
              std::cout << "Nombre registrado: " << response["Repository_name"] << std::endl;
          }
      } else {
          std::cout << "[-] No se pudo crear el repositorio." << std::endl;
          
          if (response.contains("message")) { 
              std::cout << response["message"] << std::endl;
          }
      }

      std::cout << "------------------------------------------" << std::endl;
   }
}