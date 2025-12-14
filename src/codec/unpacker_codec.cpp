#include "client/colors.hpp"
#include "client/unpacker_codec.hpp"

namespace client::unpacker { 

    bool unpack_file(const std::string& archive_path, const std::string& destination_path) {
        
        try {
             std::filesystem::create_directories(destination_path);
        } catch (...) {}

        std::string command = "tar -xf \"" + archive_path + "\" -C \"" + destination_path + "\"";

        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "\n" << client::colors::GREEN 
                      << "[+] Unpacking successful." 
                      << client::colors::RESET << std::endl;
            return true;
        } else {
            std::cerr << client::colors::RED 
                      << "[-] Error unpacking (code: " << result << ")." 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }

}