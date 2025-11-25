#pragma once

#include <string>
#include <eccrypto.h>   // << NECESARIO para CryptoPP::ECDSA
#include <osrng.h>
#include <oids.h>
#include <base64.h>
#include <files.h>

struct ECDSAKeyPair {
    std::string privateKey;
    std::string publicKey;
};

namespace client::crypto_codec {

    using PrivateKey = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;
    using PublicKey  = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey;

    ECDSAKeyPair generate_ecdsa_keypair();

    bool load_private_key(const std::string& filepath, PrivateKey& key);

    std::string sign_file(const PrivateKey& key, const std::string& filepath);

    std::string hash_sha256(const std::string& input);

}
