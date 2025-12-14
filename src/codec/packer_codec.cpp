#include "client/colors.hpp" 
#include "client/packer_codec.hpp"

namespace client::packer {

    bool pack_files(const std::vector<std::string>& files_list, const std::string& output_tar) {
        if (files_list.empty()) return false;

        std::string list_filename = "files_to_pack_temp.txt";
        std::ofstream list_file(list_filename);

        if (!list_file.is_open()) {
            std::cerr << client::colors::RED 
                      << "[Packer] Error: No se pudo crear el archivo de lista temporal." 
                      << client::colors::RESET << std::endl;
            return false;
        }
        for (const auto& file : files_list) {
            list_file << file << "\n";
        }
        list_file.close();

        // Construir comando (asegúrate que 'tar' esté en el PATH del sistema)
        std::string command = "tar -czf \"" + output_tar + "\" -T \"" + list_filename + "\"";

        int result = std::system(command.c_str());

        // Limpieza de archivo temporal
        try {
            std::filesystem::remove(list_filename);
        } catch (const std::exception& e) {
            std::cerr << client::colors::RED 
                      << "[-] No se pudo borrar el archivo temporal de lista: " << list_filename 
                      << client::colors::RESET << std::endl;
            std::cerr << client::colors::RED << "    " << e.what() << client::colors::RESET << std::endl;
        } catch (...) {
            std::cerr << client::colors::RED 
                      << "[-] No se pudo borrar el archivo temporal de lista (Error desconocido)." 
                      << client::colors::RESET << std::endl;
        }

        if (result == 0) {
            return true;
        } else {
            std::cerr << client::colors::RED 
                      << "[-] Error al empaquetar archivos (código: " << result << ")." 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }

}