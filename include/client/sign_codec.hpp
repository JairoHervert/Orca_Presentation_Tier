#pragma once

#include <iostream>

#include <osrng.h>
#include <files.h>
#include <base64.h>
#include <filters.h> 
#include <eccrypto.h>


namespace client::sign_codec {

    using ECDSAPrivateKey = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;

    bool sign_file(const ECDSAPrivateKey& key, const std::string& filepath, std::string& outSignature); 
}
