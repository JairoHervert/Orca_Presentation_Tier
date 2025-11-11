#include "client/commands.hpp"
#include <iostream>

namespace client::cmd {
   bool run_init(const std::string& repo, const std::vector<std::string>& cols) {
      std::cout << "Inicializando repositorio: " << repo << std::endl;
      std::cout << "Colaboradores:" << std::endl;
      for (const auto& col : cols) {
         std::cout << " - " << col << std::endl;
      }

      // Lógica de inicialización del repositorio iría aquí
      
      return true;
   }
}