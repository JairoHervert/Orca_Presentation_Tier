#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "client/colors.hpp"
#include "client/downloader_codec.hpp"
#include "client/http_getter.hpp"

namespace client::dowlander {

    int download_file(httplib::SSLClient& cli, const std::string &server_path, const std::string &save_path) {
        
        std::ofstream file_out(save_path, std::ios::binary);
        if (!file_out.is_open()) {
            std::cerr << client::colors::RED 
                      << "[!] Error: No se pudo abrir el archivo local para guardar: " << save_path 
                      << client::colors::RESET << std::endl;
            throw std::runtime_error("File open error");
        }

        // INFO: Cyan para la acción, normal para las rutas
        std::cout << client::colors::CYAN << "Descargando de " << client::colors::RESET << server_path 
                  << client::colors::CYAN << " a " << client::colors::RESET << save_path << std::endl;

        // Prepara las "funciones de callback" que usara el transporte
        auto data_sink = [&](const char *data, size_t length) {
            file_out.write(data, length);
            return file_out.good();
        };

        auto progress = [&](uint64_t current, uint64_t total) {
            if (total > 0) {
                // Usamos \r para sobreescribir la línea y std::flush para forzar la salida inmediata
                std::cout << "\r" << client::colors::BLUE << "Descargando... " 
                          << client::colors::YELLOW << static_cast<int>(current * 100 / total) << "%" 
                          << client::colors::RESET << std::flush;
            } else {
                std::cout << "\r" << client::colors::BLUE << "Descargando... " 
                          << client::colors::YELLOW << current << " bytes" 
                          << client::colors::RESET << std::flush;
            }
            return true;
        };

        // Llama a la capa de transporte
        int status_code = client::http_getter::get_file(cli, server_path, data_sink, progress);

        // Limpiamos la línea de progreso al terminar
        std::cout << std::endl;
        
        file_out.close();
        return status_code;
    }

}