#include "client/hasher_codec.hpp"

namespace client::hasher_codec {

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
            std::cerr << "[-] Error hashing string: " << e.what() << std::endl;
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
            std::cerr << "[-] Error hashing file: " << e.what() << std::endl;
            return "HASH_ERROR";
        }
    }

}



// ***************** B64 **************************
// #include <iostream>
// #include <stdexcept>

// #include "client/hasher_codec.hpp"

// namespace client::hasher {

//     std::string hash_sha256(const std::string& input) {
//         try {
//             CryptoPP::SHA256 hash;
//             std::string digest;

//             CryptoPP::StringSource s(input, true,
//                 new CryptoPP::HashFilter(hash,
//                     new CryptoPP::Base64Encoder(
//                         new CryptoPP::StringSink(digest), false // false = sin saltos de línea
//                     )
//                 )
//             );
//             return digest;
//         } catch (const std::exception& e) {
//             std::cerr << "[-] Error hashing string: " << e.what() << std::endl;
//             return "";
//         }
//     }

//         std::string hash_file_sha256(const std::string& filepath) {
//         try {
//             std::string b64_hash;
//             CryptoPP::SHA256 hash;
            
//             CryptoPP::FileSource s(filepath.c_str(), true,
//                 new CryptoPP::HashFilter(hash,
//                     new CryptoPP::Base64Encoder(
//                         new CryptoPP::StringSink(b64_hash), false
//                     )
//                 )
//             );
//             return b64_hash;
//         } catch (const std::exception& e) {
//             std::cerr << "[-] Error hashing file: " << e.what() << std::endl;
//             return "HASH_ERROR";
//         }
//     }

// }
