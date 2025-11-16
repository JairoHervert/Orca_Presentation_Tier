#pragma once
#include <string>

namespace client::core {

    //Descomprime un archivo .tar.gz en un directorio específico.
    bool unpack_file(const std::string& archive_path, const std::string& destination_path);

}