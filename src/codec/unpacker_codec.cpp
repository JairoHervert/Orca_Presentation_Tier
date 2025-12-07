#include "client/unpacker_codec.hpp"
#include <cstdlib>
#include <iostream>
#include <filesystem>

namespace client::unpacker { 

    bool unpack_file(const std::string& archive_path, const std::string& destination_path) {
        
        try {
             std::filesystem::create_directories(destination_path);
        } catch (...) {}

        std::string command = "tar -xf \"" + archive_path + "\" -C \"" + destination_path + "\"";


        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "\n[+] Desempaquetado exitoso." << std::endl;
            return true;
        } else {
            std::cerr << "[-] Error al desempaquetar (codigo: " << result << ")." << std::endl;
            return false;
        }
    }

}
