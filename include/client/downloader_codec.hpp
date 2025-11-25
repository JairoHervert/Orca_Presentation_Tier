#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once
#include <string>
#include <httplib.h>
#include <memory>

namespace client::dowlander {

    //Pasa la logica de descarga el .tar
    int download_file(httplib::SSLClient& cli, const std::string &server_path, const std::string &save_path);

} 