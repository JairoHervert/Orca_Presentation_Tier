#include <iostream>
#include "client/commands.hpp"
#include "client/json_codec.hpp"

namespace client::cmd {
   bool run_init(const std::string& repo, const std::vector<std::string>& cols) {
      std::cout << "Inicializando repositorio: " << repo << std::endl;
      std::cout << "Colaboradores:" << std::endl;
      for (const auto& col : cols) {
         std::cout << " - " << col << std::endl;
      }

      // Lógica de inicialización del repositorio iría aquí
      std::cout << client::json_nlohmann::make_init_payload(repo, cols).dump(4) << std::endl;

      return true;
   }
}