#pragma once

#include <iostream>
#include <vector>


#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h> 
#include <cryptopp/eccrypto.h>
// #include <osrng.h>
// #include <files.h>
// #include <base64.h>
// #include <filters.h> 
// #include <eccrypto.h>


namespace client::sign_codec {

    using ECDSAPrivateKey = CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey;

    bool sign_file_for_update(const ECDSAPrivateKey& privateKey,const std::string& hashBase64,std::string& outSignatureBase64);


    bool sign_hash_string_for_delete(const ECDSAPrivateKey& privateKey,const std::string& serverHashBase64,std::string& outSignatureBase64);

}
