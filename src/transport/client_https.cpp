#include <iostream>
#include <stdexcept>
#include <string>
#include "client/client_https.hpp"
#include "../../include/third_party/dotenv.h"

namespace client::http {

   // Retorna un puntero unico al cliente HTTPS conectado
   std::unique_ptr<httplib::SSLClient> conect() {
      try {
         // Cargar variables de entorno
         dotenv::init();
         std::string host = dotenv::getenv("HOST", "localhost");
         int port = std::stoi(dotenv::getenv("PORT", "443"));
         
         // Crear cliente HTTPS con puntero unico
         auto client_conection = std::make_unique<httplib::SSLClient>(host.c_str(), port);
         client_conection->set_read_timeout(5, 0);
         client_conection->enable_server_certificate_verification(false);
         
         return client_conection;
      } catch (const std::exception &e) {
         std::cerr << "Error al conectar: " << e.what() << std::endl;
         throw;
      }
   }

   // Enviar peticiones post HTTPS
   nlohmann::json post_json_https(const std::string &path, const nlohmann::json &payload) {
      try {
         auto cli = conect(); // Ahora es un unique_ptr
         std::string payload_str = payload.dump();
         
         // Usar el puntero con ->
         auto res = cli->Post(path.c_str(), payload_str, "application/json");
         
         if (!res || res->status != 200) {
            throw std::runtime_error("Error en la respuesta del servidor");
         }
         
         return nlohmann::json::parse(res->body);
      } catch (const std::exception &e) {
         std::cerr << "Error en post_json_https: " << e.what() << std::endl;
         throw;
      }
   }
   // Enviar peticiones get HTTPS

   // primera prueba para recibir una respuesta como string del server real
   std::string post_string_https(const std::string &path) {
      try {
         auto cli = conect(); // Ahora es un unique_ptr
         
         // Usar el puntero con ->
         auto res = cli->Post(path.c_str(), "", "application/json");
         
         if (!res || res->status != 200) {
            throw std::runtime_error("Error en la respuesta del servidor");
         }
         
         return res->body;
      } catch (const std::exception &e) {
         std::cerr << "Error en post_string_https: " << e.what() << std::endl;
         throw;
      }
   }
}
   