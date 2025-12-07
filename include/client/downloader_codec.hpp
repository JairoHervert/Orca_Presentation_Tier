#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once

#include <string>
#include <memory>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <httplib.h>
#include <stdexcept>


namespace client::dowlander {

    // Pasa la logica de descarga el .tar
    int download_file(httplib::SSLClient& cli, const std::string &server_path, const std::string &save_path);

} 