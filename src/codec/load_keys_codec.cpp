#include "client/load_keys_codec.hpp"

#pragma once

#include <string>
#include <eccrypto.h>
#include <osrng.h>
#include <oids.h>
#include <base64.h>
#include <files.h>
#include <rsa.h>

namespace client::load_keys_codec {

    bool load_private_ECDSAkey(const std::string& filepath, ECDSAPrivateKey& key) {
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

    bool load_private_OAEPkey(const std::string& KeyFile, RSAPrivateKey& key) {
        try {
           CryptoPP::AutoSeededRandomPool prng;

            CryptoPP::ByteQueue q;
            CryptoPP::FileSource fs(std::string(KeyFile).c_str(), true,
                new CryptoPP::Base64Decoder(new CryptoPP::Redirector(q))
            );

            // Cargar la llave desde el ByteQueue
            key.Load(q);

            //Validar la llave cargada
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
