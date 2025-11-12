#include "client/response_handler.hpp"

namespace client::response_handler {
   void handle_init_response(const nlohmann::json &response) {
      // por ahora solo imprimimos la respuesta
      std::cout << "Manejando respuesta de init:" << std::endl;
      std::cout << response.dump(4) << std::endl;

      // mostrar cada campo relevante
      if (response.contains("status"))
         std::cout << "Status: " << response["status"] << std::endl;

      if (response.contains("message"))
         std::cout << "Message: " << response["message"] << std::endl;

      if (response.contains("timestamp"))
         std::cout << "Timestamp: " << response["timestamp"] << std::endl;

   }
}