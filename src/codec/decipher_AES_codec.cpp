#include "client/decipher_AES_codec.hpp"

namespace client::decipher_aes {
    bool decipher_AES_GCM(const std::string &filePath, const std::string &fileOutPath, const std::string &keyAESRaw) {
        try {
            // Leer archivo cifrado
            std::ifstream inFile(filePath, std::ios::binary);
            if (!inFile) {
                std::cerr << "[-] No se pudo abrir el archivo cifrado: " << filePath << std::endl;
                return false;
            }

            // Extraer IV (primeros 12 bytes)
            CryptoPP::SecByteBlock iv(12);
            inFile.read(reinterpret_cast<char*>(iv.data()), iv.size());

            // Leer el resto
            std::string cipherText((std::istreambuf_iterator<char>(inFile)),
                                   std::istreambuf_iterator<char>());
            inFile.close();

            // Configurar descifrador
            CryptoPP::GCM<CryptoPP::AES>::Decryption decryptor;
            
            // Usamos keyAESRaw directamente
            decryptor.SetKeyWithIV(
                reinterpret_cast<const CryptoPP::byte*>(keyAESRaw.data()),
                keyAESRaw.size(),
                iv, iv.size()
            );

            // Descifrar
            std::string plainText;
            CryptoPP::StringSource ssCipher(cipherText, true,
                new CryptoPP::AuthenticatedDecryptionFilter(decryptor,
                    new CryptoPP::StringSink(plainText)
                )
            );

            // Guardar
            std::ofstream outFile(fileOutPath, std::ios::binary);
            if (!outFile) {
                std::cerr << "[-] No se pudo crear el archivo de salida." << std::endl;
                return false;
            }
            outFile.write(plainText.data(), plainText.size());
            outFile.close();

            return true;

        } catch (const CryptoPP::Exception &e) {
            std::cerr << "[-] Error Crypto++: " << e.what() << std::endl;
            return false;
        } catch (const std::exception &e) {
            std::cerr << "[-] Error: " << e.what() << std::endl;
            return false;
        }
    }
}