#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../third_party/json.hpp"

#pragma once
#include <regex>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include <httplib.h>

namespace client::http {

   // Se conecta al servidor y devuelve un puntero al cliente SSL.
   std::unique_ptr<httplib::SSLClient> conect();
   
   // Cabecera para enviar un post request HTTPS con payload JSON y retornar la respuesta JSON
   nlohmann::json post_json_https(const std::string &path, const nlohmann::json &payload);

   // Envía un JSON vía POST y guarda la respuesta local.
   nlohmann::json post_download_file(const std::string& path, const nlohmann::json& payload, const std::string& default_path);
   
   // Sube un archivo .tar junto con un JSON vía POST y retorna la respuesta JSON.
    nlohmann::json upload_push_data(const std::string &path,  const std::string &repoName, const std::string &userEmail, const std::string &userPassword, const std::string &operationsJsonStr, const std::string &tar_filepath);

   // Cabecera para enviar un get request HTTPS y retornar la respuesta JSON
   nlohmann::json get_json_https(const std::string &path);
}