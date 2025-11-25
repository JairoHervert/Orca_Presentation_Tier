#include "client/crypto_codec.hpp"

#include <iostream>
#include <stdexcept>

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

         CryptoPP::FileSource fs(
               filepath.c_str(), true,
               new CryptoPP::SignerFilter(prng, signer,
                  new CryptoPP::StringSink(signature)
               )
         );

         std::string encoded;
         CryptoPP::StringSource ssSig(
               signature, true,
               new CryptoPP::Base64Encoder(
                  new CryptoPP::StringSink(encoded), false
               )
         );

         return encoded;

      } catch (const std::exception& e) {
         std::cerr << "[Crypto] Error signing file: " << e.what() << std::endl;
         return "FILE_SIGN_ERROR";
      }
   }

} // namespace client::crypto_codec
