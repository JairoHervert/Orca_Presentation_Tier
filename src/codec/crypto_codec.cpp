#include "client/crypto_codec.hpp"

#include <iostream>
#include <stdexcept>
#include <eccrypto.h>
#include <osrng.h>
#include <oids.h>
#include <base64.h>
#include <files.h>
#include <rsa.h>

namespace client::crypto_codec {

   ECDSAKeyPair generate_ecdsa_keypair() {
      ECDSAKeyPair keyPair;

      CryptoPP::AutoSeededRandomPool prng;

      PrivateKey privateKey;
      PublicKey publicKey;

      privateKey.Initialize(prng, CryptoPP::ASN1::secp256r1());
      privateKey.MakePublicKey(publicKey);

      if (!privateKey.Validate(prng, 3) || !publicKey.Validate(prng, 3)) {
         throw std::runtime_error("Invalid ECC keypair");
      }

      // ===== PRIVATE KEY BASE64 =====
      std::string privateDer;
      CryptoPP::StringSink sinkPriv(privateDer);
      privateKey.Save(sinkPriv);

      CryptoPP::StringSource ssPriv(
         privateDer, true,
         new CryptoPP::Base64Encoder(
               new CryptoPP::StringSink(keyPair.privateKey), false
         )
      );

      // ===== PUBLIC KEY BASE64 =====
      std::string publicDer;
      CryptoPP::StringSink sinkPub(publicDer);
      publicKey.Save(sinkPub);

      CryptoPP::StringSource ssPub(
         publicDer, true,
         new CryptoPP::Base64Encoder(
               new CryptoPP::StringSink(keyPair.publicKey), false
         )
      );

      return keyPair;
   }

   bool load_private_key(const std::string& filepath, PrivateKey& key) {
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
               std::cerr << "[Crypto] La llave cargada es invalida." << std::endl;
               return false;
           }
           return true;

       } catch (const std::exception& e) {
           std::cerr << "[Crypto] Error cargando llave: " << e.what() << std::endl;
           return false;
       }
   }

   std::string sign_file(const PrivateKey& key, const std::string& filepath) {
      try {
         CryptoPP::AutoSeededRandomPool prng;
         CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(key);

         std::string signature;

         // 1. Leer y Firmar el archivo (SHA256 + ECDSA)
         // FileSource maneja la apertura y cierre del archivo automáticamente
         CryptoPP::FileSource fs(
             filepath.c_str(), true,
             new CryptoPP::SignerFilter(prng, signer,
                new CryptoPP::StringSink(signature)
             )
         );

         // 2. Codificar a Base64 para enviar en JSON
         std::string encoded;
         CryptoPP::StringSource ssSig(
             signature, true,
             new CryptoPP::Base64Encoder(
                new CryptoPP::StringSink(encoded), false // false = sin saltos de línea
             )
         );

         return encoded;

      } catch (const std::exception& e) {
         std::cerr << "[Crypto] Error signing file: " << e.what() << std::endl;
         return "";
      }
   }

   RSAKeyPair generate_rsa_keypair() {
       RSAKeyPair kp;
       try {
           CryptoPP::AutoSeededRandomPool prng;
           
           // Parámetros RSA
           CryptoPP::InvertibleRSAFunction params;
           params.GenerateRandomWithKeySize(prng, 2048);

           CryptoPP::RSA::PrivateKey privateKey(params);
           CryptoPP::RSA::PublicKey publicKey(params);

           // Validar
           if (!privateKey.Validate(prng, 3) || !publicKey.Validate(prng, 3)) {
               throw std::runtime_error("Error: Llaves RSA generadas invalidas.");
           }

           // Guardar Privada en String (Base64)
           std::string privStr;
           CryptoPP::StringSink sinkPriv(privStr);
           CryptoPP::Base64Encoder privEncoder(new CryptoPP::StringSink(kp.privateKey), false);
           privateKey.Save(privEncoder);
           privEncoder.MessageEnd();

           // Guardar Pública en String (Base64)
           std::string pubStr;
           CryptoPP::StringSink sinkPub(pubStr);
           CryptoPP::Base64Encoder pubEncoder(new CryptoPP::StringSink(kp.publicKey), false);
           publicKey.Save(pubEncoder);
           pubEncoder.MessageEnd();

           return kp;

       } catch (const std::exception &e) {
           std::cerr << "[Crypto] Error generando llaves RSA: " << e.what() << std::endl;
           throw;
       }
   }

} // namespace client::crypto_codec
