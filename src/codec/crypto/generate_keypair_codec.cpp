#include "client/colors.hpp" 
#include "client/generate_keypair_codec.hpp"

namespace client::generate_keypair_codec {

   ECDSAKeyPair generate_ecdsa_keypair() {
      ECDSAKeyPair ECDSAkeyPair;

      CryptoPP::AutoSeededRandomPool prng;

      ECDSAPrivateKey ECDSAprivateKey;
      ECDSAPublicKey ECDSApublicKey;

      ECDSAprivateKey.Initialize(prng, CryptoPP::ASN1::secp256r1());
      ECDSAprivateKey.MakePublicKey(ECDSApublicKey);

      if (!ECDSAprivateKey.Validate(prng, 3) || !ECDSApublicKey.Validate(prng, 3)) {
         std::cerr << client::colors::RED 
                   << "[!] Critical Error: Invalid ECC keypair validation failed." 
                   << client::colors::RESET << std::endl;
         throw std::runtime_error("[!] Invalid ECC keypair");
      }

      // ===== PRIVATE KEY BASE64 =====
      std::string privateDer;
      CryptoPP::StringSink sinkPriv(privateDer);
      ECDSAprivateKey.Save(sinkPriv);

      CryptoPP::StringSource ssPriv(
         privateDer, true,
         new CryptoPP::Base64Encoder(
               new CryptoPP::StringSink(ECDSAkeyPair.ECDSAprivateKey), false
         )
      );

      // ===== PUBLIC KEY BASE64 =====
      std::string publicDer;
      CryptoPP::StringSink sinkPub(publicDer);
      ECDSApublicKey.Save(sinkPub);

      CryptoPP::StringSource ssPub(
         publicDer, true,
         new CryptoPP::Base64Encoder(
               new CryptoPP::StringSink(ECDSAkeyPair.ECDSApublicKey), false
         )
      );

      return ECDSAkeyPair;
   }

   RSAKeyPair generate_rsa_keypair() {
       RSAKeyPair RSAkeyPair;
       try {
           CryptoPP::AutoSeededRandomPool prng;
           
           // RSA Parameters
           CryptoPP::InvertibleRSAFunction params;
           params.GenerateRandomWithKeySize(prng, 2048);

            RSAPrivateKey RSAprivateKey(params);
            RSAPublicKey RSApublicKey(params);


           // Validate
           if (!RSAprivateKey.Validate(prng, 3) || !RSApublicKey.Validate(prng, 3)) {
               throw std::runtime_error("[-] Error Invalid RSA keypair generated.");
           }

           // Guardar Privada en String (Base64)
           std::string privStr;
           CryptoPP::StringSink sinkPriv(privStr);
           CryptoPP::Base64Encoder privEncoder(new CryptoPP::StringSink(RSAkeyPair.RSAprivateKey), false);
           RSAprivateKey.Save(privEncoder);
           privEncoder.MessageEnd();

           // Guardar Pública en String (Base64)
           std::string pubStr;
           CryptoPP::StringSink sinkPub(pubStr);
           CryptoPP::Base64Encoder pubEncoder(new CryptoPP::StringSink(RSAkeyPair.RSApublicKey), false);
           RSApublicKey.Save(pubEncoder);
           pubEncoder.MessageEnd();

           return RSAkeyPair;

       } catch (const std::exception &e) {
           std::cerr << "\n" << client::colors::RED 
                     << "[-] Error generating RSA keys: " << e.what() 
                     << client::colors::RESET << std::endl;
           throw;
       }
   }

} // namespace client::generate_keypair_codec