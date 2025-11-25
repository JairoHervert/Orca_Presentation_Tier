#include <cstdlib> 
#include <iostream>

#include "client/unpacker_codec.hpp"

namespace client::unpacker {

    bool unpack_file(const std::string& archive_path, const std::string& destination_path) {
        
        // Construir el comando 'tar'
        // -x : eXtract (extraer)
        // -z : gZip (manejar descompresion .gz)
        // -f : File (nombre del archivo)
        // -C : Change directory (el directorio de destino)
        std::string command = "tar -xzf \"" + archive_path + "\" -C \"" + destination_path + "\"";

        std::cout << "Descomprimiendo " << archive_path << "..." << std::endl;
        std::cout << "Ejecutando: " << command << std::endl;

        // Ejecutar el comando en el sistema operativo
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Descompresion exitosa." << std::endl;
            return true;
        } else {
            std::cerr << "Error: Falla al descomprimir el archivo (codigo: " << result << ")." << std::endl;
            return false;
        }
    }

} // namespace client::core