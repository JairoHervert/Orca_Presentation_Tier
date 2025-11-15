#include <iostream>
#include "client/commands.hpp"
#include "client/json_codec.hpp"

namespace client::cmd {
   bool run_clone(const std::string& repo_name, const std::string& destination) {
      std::cout << "Clonando repositorio: " << repo_name << std::endl;
      std::cout << "Directorio de destino: " << destination << std::endl;

      // Impresión de jason creado
      std::cout << client::json_nlohmann::make_clone_payload(repo_name, destination).dump(4) << std::endl;

      return true;
   }
}