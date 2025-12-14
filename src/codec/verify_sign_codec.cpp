#include "client/colors.hpp"
#include "client/verify_sign_codec.hpp"

namespace client::verify_sign_codec {

    bool load_public_key_from_base64(const std::string& base64Key, ECDSAPublicKey& pubKey) {
        try {
            CryptoPP::AutoSeededRandomPool prng;
            std::string decoded;

            // Decodificar Base64
            CryptoPP::StringSource(base64Key, true, 
                new CryptoPP::Base64Decoder(
                    new CryptoPP::StringSink(decoded)
                )
            );

            // Cargar la llave desde los datos decodificados
            CryptoPP::StringSource ss(decoded, true);
            pubKey.Load(ss);

            // Validar
            return pubKey.Validate(prng, 3);

        } catch (const std::exception& e) {
            std::cerr << client::colors::RED 
                      << "[!] Error loading public key: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        } catch (...) { 
            std::cerr << client::colors::RED 
                      << "[!] Unknown error loading public key." 
                      << client::colors::RESET << std::endl;
            return false; 
        }
    }

    bool verify_file_signature(
        const ECDSAPublicKey& pubKey,
        const std::string& filepath,
        const std::string& base64Signature
    ) {
        try {
            // Decodificar la firma de Base64 a binario (formato P1363: r||s)
            std::string signatureBin;
            CryptoPP::StringSource(
                base64Signature, 
                true, 
                new CryptoPP::Base64Decoder(new CryptoPP::StringSink(signatureBin))
            );

            // Crear el verificador
            CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier(pubKey);

            // Validar longitud de firma
            if (signatureBin.size() != verifier.SignatureLength()) {
                std::cerr << client::colors::RED 
                          << "[Verify] Error: Incorrect signature length: " 
                          << signatureBin.size() << " vs " << verifier.SignatureLength() 
                          << client::colors::RESET << std::endl;
                return false;
            }

            // Leer el contenido del archivo completo
            std::string fileContent;
            CryptoPP::FileSource(filepath.c_str(), true, 
                new CryptoPP::StringSink(fileContent)
            );

            // Verificar usando VerifyMessage (método más directo y confiable)
            bool result = verifier.VerifyMessage(
                reinterpret_cast<const CryptoPP::byte*>(fileContent.data()),
                fileContent.size(),
                reinterpret_cast<const CryptoPP::byte*>(signatureBin.data()),
                signatureBin.size()
            );

            return result;

        } catch (const std::exception& e) {
            std::cerr << client::colors::RED 
                      << "[Verify] Exception: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }
}