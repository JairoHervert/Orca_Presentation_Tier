#pragma once

#include <iostream>

#include <eccrypto.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>


namespace client::sign_codec {

    using ECDSAPrivateKey = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;

    bool sign_file(const ECDSAPrivateKey& key, const std::string& filepath, std::string& outSignature); 
}
