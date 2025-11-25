#include <iostream>
#include <filesystem>
#include <algorithm>
#include <map>
#include <vector>

#include "client/scanner_codec.hpp"
#include "client/hasher_codec.hpp" 
namespace { 
    // Lógica de .orcaignore (simplificada)
    bool should_ignore(const std::string& path) {
        return path.find(".git") != std::string::npos || 
               path.find(".orca") != std::string::npos ||
               path.find("build") != std::string::npos;
    }

    void scan_recursive(const std::filesystem::path& current_path,
                        const std::filesystem::path& base_path,
                        std::map<std::string, std::string>& file_map) {
        for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
            std::string rel_path = std::filesystem::relative(entry.path(), base_path).string();
            std::replace(rel_path.begin(), rel_path.end(), '\\', '/');

            if (should_ignore(rel_path)) continue;

            if (entry.is_directory()) {
                scan_recursive(entry.path(), base_path, file_map);
            } else if (entry.is_regular_file()) {
                // Usamos la función separada
                file_map[rel_path] = client::hasher::hash_file_sha256(entry.path().string());
            }
        }
    }
}

namespace client::scanner {
    std::map<std::string, std::string> generate_file_map(const std::string& base_path_str) {
        std::map<std::string, std::string> file_map;
        std::filesystem::path base_path(base_path_str);
        if (std::filesystem::exists(base_path)) {
            scan_recursive(base_path, base_path, file_map);
        }
        return file_map;
    }
}
