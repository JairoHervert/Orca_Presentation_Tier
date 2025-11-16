#include <iostream>
#include <stdexcept>
#include <string>
#include "client/client_https.hpp"
#include "../../include/third_party/dotenv.h"

namespace client::http {

   // Retorna un puntero único al cliente HTTPS conectado
   std::unique_ptr<httplib::SSLClient> conect() {
      try {
         // Cargar variables de entorno
         dotenv::init();
         std::string host = dotenv::getenv("HOST", "localhost");
         int port = std::stoi(dotenv::getenv("PORT", "443"));
         
         // Crear cliente HTTPS con puntero único
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
   
   // Descarga de .tar
   bool download_file_https(const std::string &path, const std::string &save_path) {
      try {
         auto cli = conect(); 

         std::ofstream file_out(save_path, std::ios::binary);
         if (!file_out.is_open()) {
               throw std::runtime_error("Error: No se pudo abrir el archivo local para guardar: " + save_path);
         }

         std::cout << "Descargando de " << path << " a " << save_path << std::endl;

         // Peticion GET
         auto res = cli->Get(path.c_str(),
               // Receptor de contenido
               [&](const char *data, size_t length) {
                  file_out.write(data, length);
                  return file_out.good();
               },
               // Progreso de descarga
               [&](uint64_t current, uint64_t total) {
                  if (total > 0) {
                     printf("\rDescargando... %d%%", static_cast<int>(current * 100 / total));
                  } else {
                     printf("\rDescargando... %llu bytes", current);
                  }
                  return true;
               }
         );

         printf("\n");
         file_out.close();


         if (!res) {
               return false;
         }

         if (res->status == 200) {
               std::cout << "Descarga completada." << std::endl;
               return true;
         } else {
               std::cerr << "Error del servidor al descargar (Status " << res->status << ")" << std::endl;
               // Si falla, borra el archivo incompleto 
               std::remove(save_path.c_str());
               return false;
         }

      } catch (const std::exception &e) {
         std::cerr << "Error en download_file_https: " << e.what() << std::endl;
         // Si falla, borramos el archivo incompleto
         std::remove(save_path.c_str());
         return false;
      }
      return false;
   }
}