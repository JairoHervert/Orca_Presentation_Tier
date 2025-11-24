#include "client/response_handler.hpp"

namespace client::response_handler {
   void handle_keygen_response(const nlohmann::json &response) {
      // por ahora solo imprimimos la respuesta
      std::cout << "Manejando respuesta de keygen:" << std::endl;
      std::cout << response.dump(4) << std::endl;

      // mostrar cada campo relevante
      if (response.contains("key_saved"))
         std::cout << "key_saved: " << response["key_saved"] << std::endl;

      if (response.contains("status"))
         std::cout << "status: " << response["status"] << std::endl;

      if (response.contains("user_email"))
         std::cout << "user_email: " << response["user_email"] << std::endl;

   }
}