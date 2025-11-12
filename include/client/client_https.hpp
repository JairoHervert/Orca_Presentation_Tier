#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once
#include <httplib.h>
#include "../third_party/json.hpp"

namespace client::http {

   // Cabecera para enviar un post request HTTPS con payload JSON y retornar la respuesta JSON
   nlohmann::json post_json_https(const std::string &path, const nlohmann::json &payload);

}