#include <iostream>
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {
   bool run_init(const std::string& repo, const std::vector<std::string>& cols) {
      std::cout << "Inicializando repositorio: " << repo << std::endl;
      std::cout << "Colaboradores:" << std::endl;
      for (const auto& col : cols) {
         std::cout << " - " << col << std::endl;
      }

      // Lógica de inicialización del repositorio iría aquí
      std::cout << client::json_nlohmann::make_init_payload(repo, cols).dump(4) << std::endl;
      
      // enviar payload al servidor
      nlohmann::json response = client::http::post_json_https("/init", client::json_nlohmann::make_init_payload(repo, cols));

      // Procesar la respuesta del servidor
      client::response_handler::handle_init_response(response);

      return true;
   }
}