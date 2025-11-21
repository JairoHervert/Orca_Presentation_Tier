#pragma once
#include <string>
#include <vector>

namespace client::codec {

    // Crea un archivo .tar que contiene solo los archivos con modificaciones
    bool pack_files(const std::vector<std::string>& files_list, const std::string& output_tar);

}