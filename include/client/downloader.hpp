#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once
#include <string>
#include <httplib.h>
#include <memory>

namespace client::core {

    //Pasa la lógica de descarga a la capa de transporte.
    int download_file(httplib::SSLClient& cli, const std::string &server_path, const std::string &save_path);

} 