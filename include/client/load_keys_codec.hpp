#pragma once

#include <string>
#include <eccrypto.h>
#include <osrng.h>
#include <oids.h>
#include <base64.h>
#include <files.h>
#include <rsa.h>

struct ECDSAKeyPair {
    std::string privateKey;
    std::string publicKey;
};

struct RSAKeyPair {
   std::string privateKey; 
   std::string publicKey; 
};

namespace client::load_keys_codec {

    using ECDSAPrivateKey = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;
    using RSAPrivateKey = CryptoPP::RSA::PrivateKey;

    bool load_private_ECDSAkey(const std::string& filepath, ECDSAPrivateKey& key);

    bool load_private_OAEPkey(const std::string& KeyFile, RSAPrivateKey& key);


}
