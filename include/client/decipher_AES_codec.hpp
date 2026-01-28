#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/gcm.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
// #include <cryptlib.h>
// #include <gcm.h>
// #include <aes.h>
// #include <filters.h>
// #include <files.h>
// #include <osrng.h>
// #include <base64.h>

namespace client::decipher_aes {

    // Función para descifrar AES-GCM 
    bool decipher_AES_GCM(const std::string &filePath, const std::string &fileOutPath, const std::string &keyAES_Base64);

}