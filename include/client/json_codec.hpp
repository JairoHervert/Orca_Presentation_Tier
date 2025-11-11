#pragma once
#include <string>
#include <vector>
#include "../third_party/json.hpp"

namespace client::json_nlohmann {

   using json = nlohmann::json;

   // Crea el payload para el comando 'init'
   json make_init_payload(const std::string &repo_name, const std::vector<std::string> &collaborators);

   // Crea el payload para el comando 'clone'
   json make_clone_payload(const std::string &repo_url, const std::string &destination);

}
