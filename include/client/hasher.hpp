#pragma once
#include <string>
#include <map> 
#include <filesystem>

namespace client::codec {

    // Busca en el directorio y devuelve un mapa de Ruta -> Hash (Faltan llaves para la firma).
    std::map<std::string, std::string> generate_file_map(const std::string& base_path);

}