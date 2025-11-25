#pragma once
#include <string>

namespace client::unpacker {

    // Descomprime un archivo .tar en un directorio especifico.
    bool unpack_file(const std::string& archive_path, const std::string& destination_path);

}