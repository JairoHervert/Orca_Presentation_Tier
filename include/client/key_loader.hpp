#pragma once

#include <string>
#include <iostream> 

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

#include "client/colors.hpp"

namespace client::key_loader {
    
    // Alias para usar 'col::' en lugar de 'client::colors::'
    namespace col = client::colors;

    using ECDSAPrivateKey = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;
    using RSAPrivateKey = CryptoPP::RSA::PrivateKey;

    // Declaración e Implementación juntas en el Header
    template <typename KeyType>
    bool load_private_key(const std::string& filepath, KeyType& key) {
        try {
            CryptoPP::AutoSeededRandomPool prng;
            
            // Usamos ByteQueue para intermediar entre Base64 y la Llave
            CryptoPP::ByteQueue queue;
            
            // Leemos el archivo -> Decodificamos Base64 -> Guardamos en Cola
            CryptoPP::FileSource fs(filepath.c_str(), true,
                new CryptoPP::Base64Decoder(
                    new CryptoPP::Redirector(queue)
                )
            );

            // Cargamos la llave desde la cola limpia
            key.Load(queue);

            if (!key.Validate(prng, 3)) {
                std::cerr << "\n" << col::RED 
                          << "[!] Loaded key is invalid." 
                          << col::RESET << std::endl;
                return false;
            }
            return true;

        } catch (const std::exception& e) {
            std::cerr << "\n" << col::RED 
                      << "[-] Error loading key: " << e.what() 
                      << col::RESET << std::endl;
            return false;
        }
    }

    // Lee el archivo, decodifica el Base64 y guarda los BYTES CRUDOS en outKey
    inline bool load_aes_key(const std::string& filepath, std::string& outKey) {
        try {
            outKey.clear();
            // FileSource lee el archivo -> Base64Decoder lo transforma a binario -> StringSink lo guarda en outKey
            CryptoPP::FileSource fs(filepath.c_str(), true,
                new CryptoPP::Base64Decoder(
                    new CryptoPP::StringSink(outKey)
                )
            );
            
            if (outKey.size() != 32) {
                std::cerr << "\n" << col::YELLOW 
                          << "[!] Warning: Loaded AES key does not appear to be 256 bits." 
                          << col::RESET << std::endl;
            }
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "\n" << col::RED 
                      << "[-] Error loading AES key: " << e.what() 
                      << col::RESET << std::endl;
            return false;
        }
    }

}