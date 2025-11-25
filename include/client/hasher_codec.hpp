#pragma once
#include <string>

// Headers de Crypto++ necesarios
#include <sha.h>
#include <hex.h>
#include <files.h>

namespace client::hasher {

    // Calcula el hash SHA-256 de un texto
    std::string hash_sha256(const std::string& input);

    // Calcula el hash SHA-256 de un archivo completo.
    std::string hash_file_sha256(const std::string& filepath);

} 