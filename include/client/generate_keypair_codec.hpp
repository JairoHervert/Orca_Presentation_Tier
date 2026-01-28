#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
// #include <eccrypto.h>
// #include <osrng.h>
// #include <oids.h>
// #include <base64.h>
// #include <files.h>
// #include <rsa.h>

struct ECDSAKeyPair {
    std::string ECDSAprivateKey;
    std::string ECDSApublicKey;
};

struct RSAKeyPair {
   std::string RSAprivateKey; 
   std::string RSApublicKey; 
};

namespace client::generate_keypair_codec {

    using ECDSAPrivateKey = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;
    using ECDSAPublicKey  = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey;

    using RSAPrivateKey = CryptoPP::RSA::PrivateKey;
    using RSAPublicKey  = CryptoPP::RSA::PublicKey;


    ECDSAKeyPair generate_ecdsa_keypair();
    RSAKeyPair generate_rsa_keypair();

}
