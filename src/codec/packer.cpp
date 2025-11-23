#include "client/packer.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>  
#include <filesystem>

namespace client::codec {

    bool pack_files(const std::vector<std::string>& files_list, const std::string& output_tar) {
        if (files_list.empty()) return false;

        std::string list_filename = "files_to_pack_temp.txt";
        std::ofstream list_file(list_filename);

        if (!list_file.is_open()) {
            std::cerr << "[Packer] Error: No se pudo crear el archivo de lista temporal." << std::endl;
            return false;
        }
        for (const auto& file : files_list) {
            list_file << file << "\n";
        }
        list_file.close();

        std::string command = "tar -czf \"" + output_tar + "\" -T \"" + list_filename + "\"";

        int result = std::system(command.c_str());

        try {
            std::filesystem::remove(list_filename);
        } catch (const std::exception& e) {
            std::cerr << "No se pudo borrar el archivo temporal de lista: " << list_filename << std::endl;
            std::cerr << e.what() << std::endl;
        } catch (...) {
            std::cerr << " No se pudo borrar el archivo temporal de lista (Error desconocido)." << std::endl;
        }

        if (result == 0) {
            return true;
        } else {
            std::cerr << "Error al empaquetar archivos (código: " << result << ")." << std::endl;
            return false;
        }
    }

}