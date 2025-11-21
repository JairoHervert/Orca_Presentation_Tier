
#include "client/packer.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace client::codec {
    bool pack_files(const std::vector<std::string>& files_list, const std::string& output_tar) {
        if (files_list.empty()) return false;
        std::stringstream ss;
        ss << "tar -czf \"" << output_tar << "\"";
        for (const auto& file : files_list) {
            ss << " \"" << file << "\"";
        }
        return std::system(ss.str().c_str()) == 0;
    }
}
