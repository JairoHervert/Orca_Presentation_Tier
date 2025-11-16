#include <iostream>

#include "client/http_getter.hpp"

namespace client::http_getter {

        int get_file(httplib::SSLClient& cli, const std::string& server_path, DataSink data_sink, Progress progress) {
        
            // Realiza la petición GET, pasando los callbacks directamente a httplib
        auto res = cli.Get(
            server_path.c_str(),
            // Receptor de contenido
            data_sink,
            
            // Reporte de progreso
            progress
        );

        // Verifica si la petición en sí falló
        if (!res) {
            std::cerr << "Error de transporte: La petición GET falló (revisar conexión)." << std::endl;
            return -1;
        }

        // Si la petición tuvo éxito, devuelve el código de estado del servidor
        return res->status;
    }

}