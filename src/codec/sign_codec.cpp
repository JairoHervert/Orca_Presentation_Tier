#include "client/sign_codec.hpp"
#include <iostream>
#include <files.h>
#include <base64.h>
#include <osrng.h>
#include <filters.h> 

namespace client::sign_codec {

    bool sign_file(
        const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey& privateKey,
        const std::string& filepath,
        std::string& outSignatureBase64
    ) {
        try {
            CryptoPP::AutoSeededRandomPool prng;
            CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);

            std::string signature;
            
            CryptoPP::FileSource fs(
                filepath.c_str(), 
                true, 
                new CryptoPP::SignerFilter(
                    prng, 
                    signer,
                    new CryptoPP::Base64Encoder(
                        new CryptoPP::StringSink(signature),
                        false 
                    )
                )
            );

            // Validacion de longitud 
            if (signature.empty()) return false;

            outSignatureBase64 = signature;

            return true;

        } catch (const std::exception& e) {
            std::cerr << "[Sign] Error: " << e.what() << std::endl;
            return false;
        }
    }
}