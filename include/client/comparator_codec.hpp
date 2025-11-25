#pragma once
#include <string>
#include <vector>
#include <map>

namespace client::comparator {

    // Compara dos mapas de archivos (local vs remoto) y determina que subir.
    std::vector<std::string> compute_diff(
        const std::map<std::string, std::string>& local_map, 
        const std::map<std::string, std::string>& remote_map
    );

}