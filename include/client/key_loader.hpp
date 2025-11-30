#pragma once

#include <string>
#include <iostream> 

#include <eccrypto.h>
#include <osrng.h>
#include <oids.h>
#include <base64.h>
#include <files.h>
#include <rsa.h>

namespace client::key_loader {
    
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
                std::cerr << "[!] La llave cargada es invalida." << std::endl;
                return false;
            }
            return true;

        } catch (const std::exception& e) {
            std::cerr << "[-] Error cargando llave: " << e.what() << std::endl;
            return false;
        }
    }

}