#include <iostream>

#include "client/comparator_codec.hpp"

namespace client::comparator {
    std::vector<std::string> compute_diff(
        const std::map<std::string, std::string>& local_map, 
        const std::map<std::string, std::string>& remote_map) {
        
        std::vector<std::string> files_to_upload;
        for (const auto& [path, local_hash] : local_map) {
            auto it = remote_map.find(path);
            if (it != remote_map.end()) {
                if (local_hash != it->second) {
                    std::cout << "  [MODIFICADO] " << path << std::endl;
                    files_to_upload.push_back(path);
                }
            } else {
                std::cout << "  [NUEVO]      " << path << std::endl;
                files_to_upload.push_back(path);
            }
        }
        return files_to_upload;
    }
}