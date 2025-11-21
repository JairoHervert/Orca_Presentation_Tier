#include "client/hasher.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <map>
#include <sha.h>
#include <hex.h>
#include <files.h>

namespace { 
    std::string calculate_file_hash(const std::string& full_path) {
        try {
            std::string hex_hash;
            CryptoPP::SHA256 hash;
            CryptoPP::FileSource(full_path.c_str(), true,
                new CryptoPP::HashFilter(hash,
                    new CryptoPP::HexEncoder(new CryptoPP::StringSink(hex_hash), false)
                )
            );
            return hex_hash;
        } catch (...) { return "HASH_ERROR"; }
    }

    void scan_recursive(const std::filesystem::path& current_path,
                        const std::filesystem::path& base_path,
                        std::map<std::string, std::string>& file_map) {
        for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
            if (entry.is_directory()) {
                scan_recursive(entry.path(), base_path, file_map);
            } else if (entry.is_regular_file()) {
                std::string relative_path = std::filesystem::relative(entry.path(), base_path).string();
                std::replace(relative_path.begin(), relative_path.end(), '\\', '/');
                file_map[relative_path] = calculate_file_hash(entry.path().string());
            }
        }
    }
} 

namespace client::codec {
    std::map<std::string, std::string> generate_file_map(const std::string& base_path_str) {
        std::map<std::string, std::string> file_map;
        std::filesystem::path base_path(base_path_str);
        if (std::filesystem::exists(base_path)) {
            scan_recursive(base_path, base_path, file_map);
        }
        return file_map;
    }
}