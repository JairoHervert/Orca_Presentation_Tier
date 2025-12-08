#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../third_party/json.hpp"

#pragma once
#include <httplib.h>
#include <memory>

namespace client::http {

   // Cabecera para enviar un post request HTTPS con payload JSON y retornar la respuesta JSON
   nlohmann::json post_json_https(const std::string &path, const nlohmann::json &payload);

   // Envía un JSON vía POST y guarda la respuesta local.
   nlohmann::json post_download_file(const std::string& path, const nlohmann::json& payload, const std::string& default_path);
   
   // Sube un archivo .tar junto con un JSON vía POST y retorna la respuesta JSON.
    nlohmann::json upload_push_data(const std::string &path,  const std::string &repoName, const std::string &userEmail, const std::string &userPassword, const std::string &operationsJsonStr, const std::string &tar_filepath);

   // Cabecera para enviar un get request HTTPS y retornar la respuesta JSON
   nlohmann::json get_json_https(const std::string &path);


   // ***** PROBABLEMENTE SE QUITEN *************
   // primera prueba para recibir una respuesta como string del server real
   std::string post_string_https(const std::string &path);
   // Se conecta al servidor y devuelve un puntero al cliente SSL.
   std::unique_ptr<httplib::SSLClient> conect();
   // Subir un archivo .tar y un JSON al mismo tiempo (Multipart)
}