#pragma once

#include <string>

struct ECDSAKeyPair {
   std::string privateKey; // Clave privada en formato base64
   std::string publicKey;  // Clave pública en formato base64
};

namespace client::crypto_codec {

   // Genera un par de claves ECDSA (privada y pública en Base64)
   ECDSAKeyPair generate_ecdsa_keypair();

} // namespace client::crypto_codec
