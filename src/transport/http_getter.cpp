#include <iostream>

#include "client/http_getter.hpp"

namespace client::http_getter {

        int get_file(httplib::SSLClient& cli, const std::string& server_path, DataSink data_sink, Progress progress) {
        
            // Realiza la peticion GET, pasando los callbacks directamente a httplib
        auto res = cli.Get(
            server_path.c_str(),
            // Receptor de contenido
            data_sink,
            
            // Reporte de progreso
            progress
        );

        // Verifica si la peticion en si fallo
        if (!res) {
            std::cerr << "Error de transporte: La peticion GET fallo (revisar conexion)." << std::endl;
            return -1;
        }

        // Si la peticion tuvo exito, devuelve el codigo de estado del servidor
        return res->status;
    }

}