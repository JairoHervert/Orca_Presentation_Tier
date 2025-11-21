#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <fstream>
#include <cstdio>
#include <iostream>
#include <stdexcept>

#include "client/downloader.hpp"
#include "client/http_getter.hpp"

namespace client::codec {

    // Devuelve un INT
    int download_file(httplib::SSLClient& cli, const std::string &server_path, const std::string &save_path) {
        
        // Prepara el archivo de salida
        std::ofstream file_out(save_path, std::ios::binary);
        if (!file_out.is_open()) {
            throw std::runtime_error("Error: No se pudo abrir el archivo local para guardar: " + save_path);
        }

        std::cout << "Descargando de " << server_path << " a " << save_path << std::endl;

        // Prepara las "funciones de callback" que usara el transporte
        auto data_sink = [&](const char *data, size_t length) {
            file_out.write(data, length);
            return file_out.good();
        };

        auto progress = [&](uint64_t current, uint64_t total) {
            if (total > 0) {
                printf("\rDescargando... %d%%", static_cast<int>(current * 100 / total));
            } else {
                printf("\rDescargando... %llu bytes", current);
            }
            return true;
        };

        // Llama a la capa de transporte
        int status_code = client::http_getter::get_file(cli, server_path, data_sink, progress);

        printf("\n");
        file_out.close();
        return status_code;
    }

} 