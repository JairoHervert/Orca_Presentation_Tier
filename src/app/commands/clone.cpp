#include <iostream>
#include "client/commands.hpp"
#include "client/json_codec.hpp"

namespace client::cmd {
   bool run_clone(const std::string& repo_url, const std::string& destination) {
      std::cout << "Clonando repositorio desde: " << repo_url << std::endl;
      std::cout << "Directorio de destino: " << destination << std::endl;

      // Lógica de clonación del repositorio iría aquí
      std::cout << client::json_nlohmann::make_clone_payload(repo_url, destination).dump(4) << std::endl;

      return true;
   }
}