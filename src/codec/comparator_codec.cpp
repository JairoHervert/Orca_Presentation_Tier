#include "client/colors.hpp"
#include "client/comparator_codec.hpp"

namespace client::comparator {
    DiffResult compute_diff(
        const std::map<std::string, std::string>& local_map, 
        const std::map<std::string, std::string>& remote_map) 
    {
        DiffResult result;

        // Find Modified and New (Iterate Local)
        for (const auto& [path, local_hash] : local_map) {
            auto it = remote_map.find(path);
            if (it != remote_map.end()) {
                if (local_hash != it->second) {
                    std::cout << "    " << client::colors::YELLOW 
                              << "[MODIFIED] " << client::colors::RESET 
                              << path << std::endl;
                    result.to_upload.push_back(path);
                }
            } else {
                std::cout << "    " << client::colors::GREEN 
                          << "[NEW]      " << client::colors::RESET 
                          << path << std::endl;
                result.to_upload.push_back(path);
            }
        }

        // If it is in remote but NOT in local, the user deleted it.
        for (const auto& [remote_path, _] : remote_map) {
            if (local_map.find(remote_path) == local_map.end()) {
                std::cout << "    " << client::colors::RED 
                          << "[DELETED]  " << client::colors::RESET 
                          << remote_path << std::endl;
                result.to_delete.push_back(remote_path);
            }
        }

        return result;
    }
}