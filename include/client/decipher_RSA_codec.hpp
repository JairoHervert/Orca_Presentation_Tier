#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <cryptlib.h>
#include <integer.h>
#include <rsa.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>
#include <hex.h>
#include <sha.h>

namespace client::decipher_RSA_codec{
    using RSAPrivateKey = CryptoPP::RSA::PrivateKey;
    
    bool OAEP_decryptFile(RSAPrivateKey &privKeyOAEP, const std::string &cipherText, std::string& outPlaintext);


}