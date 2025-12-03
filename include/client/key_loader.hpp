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
                std::cerr << "[!] Advertencia: La llave AES cargada no parece ser de 256 bits." << std::endl;
            }
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "[-] Error cargando llave AES: " << e.what() << std::endl;
            return false;
        }
    }

}