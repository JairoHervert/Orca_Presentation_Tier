#pragma once
#include <string>

namespace client::files_codec {

   // Guarda una cadena en un archivo. Devuelve true si tuvo éxito.
   bool save_string_to_file(const std::string &content, const std::string &filePath);

} // namespace client::files_codec

