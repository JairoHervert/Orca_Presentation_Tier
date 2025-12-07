#pragma once

#include <iostream>
#include <stdexcept>
#include <string>


#include <sha.h>
#include <hex.h>
#include <base64.h>
#include <files.h>

namespace client::hasher_codec {

    // Calcula el hash SHA-256 de un texto
    std::string hash_sha256(const std::string& input);

    // Calcula el hash SHA-256 de un archivo completo.
    std::string hash_file_sha256(const std::string& filepath);

} 