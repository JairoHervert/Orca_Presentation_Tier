#pragma once
#include <string>
#include <vector>
#include "../third_party/json.hpp"

namespace client::json_nlohmann {

   using json = nlohmann::json;

   // Crea el payload para el comando 'init'
   json make_init_payload(const std::string &repo_name, const std::vector<std::string> &collaborators);

   // Crea el payload para el comando 'clone'
   json make_clone_payload(const std::string &repo_name, const std::string &destination); 
   
   // Crea el payload para el comando 'push' (Solicita los hashes)
   json make_push_check_payload(const std::string &project_name);

   // Crea el payload para el comando 'push' (Envia .tar y metadata)
   json make_push_upload_payload(const std::string &project_name, const std::map<std::string, std::string>& files_map);

   // Crea el payload para el comando 'config' 
   json make_config_payload(const std::string &name, const std::string &email);

   // Crea el payload para el comando 'log' 
   json make_log_payload(const std::string &project_name);

   // Crea el payload para el comando 'revoke' 
   json make_revoke_payload(const std::string &project_name, const std::string &email, const std::string &file);
}
