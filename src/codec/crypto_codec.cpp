// #include "client/crypto_codec.hpp"

// #include <iostream>
// #include <stdexcept>
// #include <eccrypto.h>
// #include <osrng.h>
// #include <oids.h>
// #include <base64.h>
// #include <files.h>

#include "client/crypto_codec.hpp"

#include <iostream>
#include <stdexcept>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>

namespace client::crypto_codec {

   ECDSAKeyPair generate_ecdsa_keypair() {
      ECDSAKeyPair keyPair;

      try {
         CryptoPP::AutoSeededRandomPool prng;

         // Crear el par de claves usando la curva secp256r1 (P-256)
         CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
         CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey  publicKey;

         // Generar la clave privada
         privateKey.Initialize(prng, CryptoPP::ASN1::secp256r1());

         // Derivar la clave pública
         privateKey.MakePublicKey(publicKey);

         // Validar las claves
         bool privadaValida = privateKey.Validate(prng, 3);
         bool publicaValida = publicKey.Validate(prng, 3);

         if (!privadaValida || !publicaValida) {
            throw std::runtime_error("Las claves generadas no son válidas");
         }

         // ===== Clave privada -> DER -> Base64 =====
         std::string clavePrivadaDER;
         CryptoPP::StringSink sinkPriv(clavePrivadaDER);
         privateKey.Save(sinkPriv);

         CryptoPP::StringSource ssPriv(
            clavePrivadaDER,
            true,
            new CryptoPP::Base64Encoder(
               new CryptoPP::StringSink(keyPair.privateKey),
               false // sin saltos de línea
            )
         );

         // ===== Clave pública -> DER -> Base64 =====
         std::string clavePublicaDER;
         CryptoPP::StringSink sinkPub(clavePublicaDER);
         publicKey.Save(sinkPub);

         CryptoPP::StringSource ssPub(
            clavePublicaDER,
            true,
            new CryptoPP::Base64Encoder(
               new CryptoPP::StringSink(keyPair.publicKey),
               false // sin saltos de línea
            )
         );

         return keyPair;
      }
      catch (const CryptoPP::Exception &e) {
         std::cerr << "Error generando par de claves ECDSA: "
                   << e.what() << std::endl;
         throw;
      }
   }

} // namespace client::crypto_codec