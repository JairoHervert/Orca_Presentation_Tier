#include <iostream>
#include <stdexcept>

#include "client/hasher_codec.hpp"

namespace client::hasher {

    std::string hash_sha256(const std::string& input) {
        try {
            CryptoPP::SHA256 hash;
            std::string digest;

            CryptoPP::StringSource s(input, true,
                new CryptoPP::HashFilter(hash,
                    new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest), false) 
                )
            );
            return digest;
        } catch (const std::exception& e) {
            std::cerr << "[HasherUtils] Error hasheando string: " << e.what() << std::endl;
            return "";
        }
    }

    std::string hash_file_sha256(const std::string& filepath) {
        try {
            std::string hex_hash;
            CryptoPP::SHA256 hash;
            
            CryptoPP::FileSource s(filepath.c_str(), true,
                new CryptoPP::HashFilter(hash,
                    new CryptoPP::HexEncoder(new CryptoPP::StringSink(hex_hash), false)
                )
            );
            return hex_hash;
        } catch (const std::exception& e) {
            std::cerr << "[HasherUtils] Error hasheando archivo: " << e.what() << std::endl;
            return "HASH_ERROR";
        }
    }

}