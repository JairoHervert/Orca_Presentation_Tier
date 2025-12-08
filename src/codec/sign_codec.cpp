#include "client/sign_codec.hpp"
#include "client/hasher_codec.hpp"

namespace client::sign_codec {
    
    // FIRMA PARA UPDATE
    bool sign_file_for_update(const ECDSAPrivateKey& privateKey,const std::string& hashBase64,std::string& outSignatureBase64) {
        try {
            CryptoPP::AutoSeededRandomPool prng;
            
            // A. Decodificar el Hash Base64 a Digest Binario (Bytes crudos)
            std::string digest;
            CryptoPP::StringSource ss(hashBase64, true,
                new CryptoPP::Base64Decoder(
                    new CryptoPP::StringSink(digest)
                )
            );

            // B. Firmar el Digest Binario
            CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);
            
            size_t sigLen = signer.MaxSignatureLength();
            std::vector<CryptoPP::byte> signature(sigLen);

            // Nota: Aquí firmamos 'digest', que son los 32 bytes crudos del SHA256
            sigLen = signer.SignMessage(
                prng,
                reinterpret_cast<const CryptoPP::byte*>(digest.data()),
                digest.size(),
                signature.data()
            );
            signature.resize(sigLen);

            // C. Encode Base64 de la firma (para el JSON)
            CryptoPP::StringSource ssEnc(signature.data(), signature.size(), true,
                new CryptoPP::Base64Encoder(
                    new CryptoPP::StringSink(outSignatureBase64), false
                )
            );
            return true;

        } catch (const std::exception& e) {
            std::cerr << "\n[-] Error signing update hash: " << e.what() << std::endl;
            return false;
        }
    }

    // FIRMA PARA DELETE 
    bool sign_hash_string_for_delete(const ECDSAPrivateKey& privateKey,const std::string& serverHashBase64,std::string& outSignatureBase64) {
        try {
            CryptoPP::AutoSeededRandomPool prng;
            CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);

            std::string signature;
            
            // Firmamos el TEXTO hashBase64 directamente
            CryptoPP::StringSource ss(serverHashBase64, true,
                new CryptoPP::SignerFilter(prng, signer,
                    new CryptoPP::StringSink(signature)
                )
            );

            // Encode Base64 de la firma
            CryptoPP::StringSource ssEnc(signature, true,
                new CryptoPP::Base64Encoder(
                    new CryptoPP::StringSink(outSignatureBase64), false
                )
            );
            return true;

        } catch (const std::exception& e) {
            std::cerr << "\n[-] Error signing delete hash: " << e.what() << std::endl;
            return false;
        }
    }
}