#include "client/colors.hpp" 
#include "client/decipher_AES_codec.hpp"

namespace client::decipher_aes {

    bool decipher_AES_GCM(const std::string &filePath, const std::string &fileOutPath, const std::string &keyAESRaw) {
        try {
            // Leer archivo cifrado
            std::ifstream inFile(filePath, std::ios::binary);
            if (!inFile) {
                std::cerr << "\n" << client::colors::RED 
                          << "[-] Could not open encrypted file: " << filePath 
                          << client::colors::RESET << std::endl;
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
                std::cerr << "\n" << client::colors::RED 
                          << "[-] Could not create output file." 
                          << client::colors::RESET << std::endl;
                return false;
            }
            outFile.write(plainText.data(), plainText.size());
            outFile.close();

            return true;

        } catch (const CryptoPP::Exception &e) {
            std::cerr << "\n" << client::colors::RED 
                      << "[!] Crypto Error: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        } catch (const std::exception &e) {
            std::cerr << "\n" << client::colors::RED 
                      << "[!] Error: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }
}