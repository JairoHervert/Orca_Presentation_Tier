#include "client/decipher_RSA_codec.hpp"

using namespace CryptoPP;
using namespace std;

namespace client::decipher_RSA_codec {

bool OAEP_decryptFile(
    RSAPrivateKey& privKeyOAEP,
    const std::string& cipherTextB64,
    std::string& outPlaintext
) {
    try {
        AutoSeededRandomPool prng;

        // 1. Decodificar Base64
        std::string binaryCipher;
        StringSource(cipherTextB64, true,
            new Base64Decoder(
                new StringSink(binaryCipher)
            )
        );

        // Crear el decryptor OAEP-SHA256
        RSAES<OAEP<SHA256>>::Decryptor decryptor(privKeyOAEP);

        // Descifrar → se escribe en outPlaintext
        outPlaintext.clear();
        StringSource(binaryCipher, true,
            new PK_DecryptorFilter(prng, decryptor,
                new StringSink(outPlaintext)
            )
        );

        return true;
    }
    catch(const CryptoPP::Exception& e) {
        return false;
    }
    catch(const std::exception& e) {
        return false;
    }
}

}
