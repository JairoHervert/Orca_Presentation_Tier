#include "client/files_codec.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace client::files_codec {

   bool save_string_to_file(const std::string &content,
                            const std::string &filePath) {
      try {
         std::ofstream outFile(filePath, std::ios::binary);
         if (!outFile) {
            std::cerr << "[-] Error: opening file for writing: "
                      << filePath << std::endl;
            return false;
         }

         outFile << content;
         outFile.close();
         return true;
      }
      catch (const std::exception &e) {
         std::cerr << "[-] Error: saving to file: "
                   << e.what() << std::endl;
         return false;
      }
   }

} // namespace client::files_codec