#define CPPHTTPLIB_OPENSSL_SUPPORT
#pragma once
#include <httplib.h>
#include <string>
#include <functional>

namespace client::http_getter {

    // Callback para recibir los "pedazos" de datos
    // Recibe: (puntero a los datos, tamaño de los datos) -> devuelve true si todo va bien
    using DataSink = std::function<bool(const char *data, size_t length)>;
    
    // Callback para informar el progreso
    // Recibe: (bytes actuales, bytes totales) -> devuelve true para continuar
    using Progress = std::function<bool(uint64_t current, uint64_t total)>;

    // Realiza una peticion GET para descargar un archivo.
    int get_file(httplib::SSLClient& cli, const std::string& server_path, DataSink data_sink, Progress progress);

} 