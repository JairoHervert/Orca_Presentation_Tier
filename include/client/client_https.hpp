#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once
#include <httplib.h>
#include <memory>
#include "../third_party/json.hpp"

namespace client::http {

   // Cabecera para enviar un post request HTTPS con payload JSON y retornar la respuesta JSON
   nlohmann::json post_json_https(const std::string &path, const nlohmann::json &payload);

   // primera prueba para recibir una respuesta como string del server real
   std::string post_string_https(const std::string &path);

   // Se conecta al servidor y devuelve un puntero al cliente SSL.
   std::unique_ptr<httplib::SSLClient> conect();

}