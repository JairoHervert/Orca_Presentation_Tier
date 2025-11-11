#include "client/commands.hpp"
#include <iostream>

namespace client::cmd {
   bool run_clone(const std::string& repo_url, const std::string& destination) {
      std::cout << "Clonando repositorio desde: " << repo_url << std::endl;
      std::cout << "Directorio de destino: " << destination << std::endl;

      // Lógica de clonación del repositorio iría aquí

      return true;
   }
}