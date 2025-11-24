// #include "client\response_handler.hpp"
#include "client/response_handler.hpp"

namespace client::response_handler {
    void handle_clone_response(const nlohmann::json &response) {
      std::cout << "Manejando respuesta de clone:" << std::endl;
      std::cout << response.dump(4) << std::endl;

      if (response.contains("status"))
         std::cout << "Status: " << response["status"] << std::endl;

      if (response.contains("message"))
         std::cout << "Message: " << response["message"] << std::endl;

      if (response.contains("timestamp"))
         std::cout << "Timestamp: " << response["timestamp"] << std::endl;

   }
}