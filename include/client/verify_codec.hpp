#pragma once

#include <iostream>
#include <string>

#include <eccrypto.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>

namespace client::verify_codec {

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
