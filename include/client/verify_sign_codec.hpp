#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/base64.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/filters.h>
// #include <osrng.h>
// #include <files.h>
// #include <base64.h>
// #include <eccrypto.h>
// #include <filters.h>

namespace client::verify_sign_codec {

    using ECDSAPublicKey =
        CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey;

    // Carga una llave pública desde base64
    bool load_public_key_from_base64(const std::string& base64Key,
                                     ECDSAPublicKey& pubKey);

    // Verifica la firma de un archivo
    bool verify_file_signature(const ECDSAPublicKey& pubKey,
                               const std::string& filepath,
                               const std::string& base64Signature);
}
