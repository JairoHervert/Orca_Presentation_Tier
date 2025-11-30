#include "client/unpacker_codec.hpp"
#include <cstdlib> 
#include <iostream>
#include <filesystem>

namespace client::unpacker {

    bool unpack_file(const std::string& archive_path, const std::string& destination_path) {
        
        // 1. Crear el directorio de destino si no existe
        try {
             std::filesystem::create_directories(destination_path);
        } catch (...) {
             // Si falla no importa tanto, tar nos avisará, pero intentamos ayudar
        }

        // Construir el comando 'tar'
        // -x : eXtract (extraer)
        // -f : File (archivo)
        // (Quitamos la 'z' para que detecte automátiamente si es gz, xz o tar simple)
        // -C : Change directory (destino)
        
        std::string command = "tar -xf \"" + archive_path + "\" -C \"" + destination_path + "\"";

        std::cout << "[Unpacker] Desempaquetando: " << archive_path << std::endl;
        std::cout << "[Unpacker] Destino: " << destination_path << std::endl;

        // Ejecutar el comando
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "[+] Desempaquetado exitoso." << std::endl;
            return true;
        } else {
            std::cerr << "[-] Error al desempaquetar (codigo: " << result << ")." << std::endl;
            return false;
        }
    }

}