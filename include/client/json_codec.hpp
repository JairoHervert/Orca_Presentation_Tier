#pragma once
#include <string>
#include <vector>
#include "../third_party/json.hpp"

namespace client::json_nlohmann {

   using json = nlohmann::json;

   
   // Crea el payload para el comando 'config' 
   json make_config_payload(const std::string &name, const std::string &email);
   
   // Crea el payload para el comando 'init'
   json make_init_payload(const std::string &repo_name, const std::vector<std::string> &collaborators);
   
   // Crea el payload para el comando 'keygen'. Se quita el path porque ese es local y se agrega la clave publica a almacenar en el servidor
   json make_keygen_ecdsa_payload(const std::string &kPubECDSA, const std::string &email, const std::string &password);

   // Crea el payload para el comando 'clone'
   json make_clone_payload(const std::string &repo_name, const std::string &destination); 
   
   // Crea el payload para el comando 'push' (Solicita los hashes)
   json make_push_check_payload(const std::string &project_name);

   // Crea el payload para el comando 'push' (Envia .tar y metadata)
   json make_push_upload_payload(const std::string &project_name, const std::map<std::string, std::string>& files_map, const std::string& password);

   // Crea el payload para el comando 'log' 
   json make_log_payload(const std::string &project_name);

   // Crea el payload para el comando 'revoke' 
   json make_revoke_payload(const std::string &project_name, const std::string &email, const std::string &file);

   // Crea el payload para el comando 'grant' 
   json make_grant_payload(const std::string &project_name, const std::string &email, const std::string &file);

   // Crea el payload para el comando 'drop' 
   json make_drop_payload(const std::string &email);

   // Crea el payload para el comando 'active'
   json make_active_payload(const std::string &email);

   // Crea el payload para el comando 'remove'
   json make_remove_payload(const std::string &project_name);
}
