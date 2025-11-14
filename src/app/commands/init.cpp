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
      //nlohmann::json response = client::http::post_json_https("/repo/create", client::json_nlohmann::make_init_payload(repo, cols));

      // prueba para recibir una respuesta como string del server real
      std::string response = client::http::post_string_https("/repo/create");
      std::cout << "Respuesta del servidor: " << response << std::endl;


      // Procesar la respuesta del servidor
      //client::response_handler::handle_init_response(response);

      return true;
   }
}