#include <iostream>
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp" 

namespace client::cmd {

   bool run_init(const std::string& repo_name, const std::string& email, const std::string& password) {
      std::cout << std::endl << "\n --- Inicializando Nuevo Repositorio ---" << std::endl;
      std::cout << "Nombre: " << repo_name << std::endl;
      std::cout << "Owner:  " << email << std::endl;

      // Hashear password
      std::string hashedPassword = client::hasher::hash_sha256(password);
      if (hashedPassword.empty()) {
          std::cerr << "[-]] Error al procesar la contrasena." << std::endl;
          return false;
      }

      // Crear Payload
      nlohmann::json payload = client::json_nlohmann::make_init_payload(repo_name, email, hashedPassword);

      std::cout<< std::endl;

      try {
          nlohmann::json response = client::http::post_json_https("/repo/init", payload);

          client::response_handler::handle_init_response(response);
          
          if (response.contains("status") && response["status"] == "ok") {
              return true;
          }

      } catch (const std::exception &e) {
          std::cerr << "[!] Error de Conexion " << e.what() << std::endl;
          return false;
      }

      return false;
   }
}