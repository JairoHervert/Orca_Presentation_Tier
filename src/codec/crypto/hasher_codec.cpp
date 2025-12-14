#include "client/colors.hpp" 
#include "client/hasher_codec.hpp"

namespace client::hasher_codec {

    std::string hash_sha256(const std::string& input) {
        try {
            CryptoPP::SHA256 hash;
            std::string digest;

            CryptoPP::StringSource s(input, true,
                new CryptoPP::HashFilter(hash,
                    new CryptoPP::Base64Encoder(
                        new CryptoPP::StringSink(digest), false // false = sin saltos de línea
                    )
                )
            );
            return digest;
        } catch (const std::exception& e) {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error hashing string: " << e.what() 
                      << client::colors::RESET << std::endl;
            return "";
        }
    }

    std::string hash_file_sha256(const std::string& filepath) {
        try {
            std::string b64_hash;
            CryptoPP::SHA256 hash;
            
            CryptoPP::FileSource s(filepath.c_str(), true,
                new CryptoPP::HashFilter(hash,
                    new CryptoPP::Base64Encoder(
                        new CryptoPP::StringSink(b64_hash), false
                    )
                )
            );
            return b64_hash;
        } catch (const std::exception& e) {
            std::cerr << "\n" << client::colors::RED 
                      << "[-] Error hashing file: " << e.what() 
                      << client::colors::RESET << std::endl;
            return "HASH_ERROR";
        }
    }

}