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
            std::cerr << "Error al abrir el archivo para escribir: "
                      << filePath << std::endl;
            return false;
         }

         outFile << content;
         outFile.close();
         return true;
      }
      catch (const std::exception &e) {
         std::cerr << "Error al guardar en el archivo: "
                   << e.what() << std::endl;
         return false;
      }
   }

} // namespace client::files_codec
