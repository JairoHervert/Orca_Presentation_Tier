#include "client/colors.hpp"
#include "client/files_codec.hpp"

namespace client::files_codec {

   bool save_string_to_file(const std::string &content,
                            const std::string &filePath) {
      try {
         std::ofstream outFile(filePath, std::ios::binary);
         if (!outFile) {
            std::cerr << client::colors::RED 
                      << "[-] Error: opening file for writing: " << filePath 
                      << client::colors::RESET << std::endl;
            return false;
         }

         outFile << content;
         outFile.close();
         return true;
      }
      catch (const std::exception &e) {
         std::cerr << client::colors::RED 
                   << "[-] Error: saving to file: " << e.what() 
                   << client::colors::RESET << std::endl;
         return false;
      }
   }

} // namespace client::files_codec