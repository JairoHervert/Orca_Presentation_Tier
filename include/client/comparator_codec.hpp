#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>

namespace client::comparator {
    struct DiffResult {
        std::vector<std::string> to_upload;
        std::vector<std::string> to_delete;
    };
    // Compara dos mapas de archivos (local vs remoto) y determina que subir.
    DiffResult compute_diff(
        const std::map<std::string, std::string>& local_map, 
        const std::map<std::string, std::string>& remote_map);

}