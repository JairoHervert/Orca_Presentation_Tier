#pragma once
#include <string>
#include <vector>
#include "../third_party/json.hpp"

namespace client::json_nlohmann {

   using json = nlohmann::json;
   struct PushOperation {
         std::string op; 
         std::string path;
         std::string signature;
      };

   // Crea el JSON de las operaciones a realizar en el push
   inline std::string make_push_operations_json(const std::vector<PushOperation>& ops) {
      json j = json::array();
      for(const auto& item : ops) {
         j.push_back({
               {"op", item.op},
               {"path", item.path},
               {"signature", item.signature}
         });
      }
      return j.dump();
   }


   // Crea el payload para el comando 'create_user' 
   json make_create_user_payload(const std::string &name, const std::string &email, const std::string& password);
   
   // Crea el payload para el comando 'init'
   json make_init_payload(const std::string &repo_name, const std::string &owner_email, const std::string &hashed_password);
   
   // Crea el payload para el comando 'keygen'. Se quita el path porque ese es local y se agrega la clave publica a almacenar en el servidor
   json make_keygen_ecdsa_payload(const std::string &kPubECDSA, const std::string &email, const std::string &password);

   // Crea el payload para el comando 'clone'
   json make_clone_payload(const std::string &repo_name, const std::string &email, const std::string &hashed_password);
   
   // Crea el payload para el comando 'push' (Solicita los hashes)
   json make_push_check_payload(const std::string& repo_name, const std::string& email, const std::string& hashed_password);
   
   // Crea el payload para el comando 'push' (Envia .tar y metadata)
   json make_push_upload_payload(const std::string& repo_name, const std::string& email, const std::string& hashed_password, const std::map<std::string, std::string>& signatures);

   // Crea el payload para el comando 'log' 
   json make_log_payload(const std::string &project_name);

   // Verifica un usuario en el sistema 
   json make_verify_user_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email);

   // Cambia el role de un usuario 
   json make_set_role_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email, int new_role);

   // Cambia el status de un usuario
   json make_set_status_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email, int new_status);
   
   // Generar Claves RSA_OAEP
   json make_keygen_rsa_payload(const std::string &public_key, const std::string &email, const std::string &hashed_password);

   // Cifra un repositorio 
   json make_encrypt_repo_payload(const std::string& leader_email, const std::string& leader_password, const std::string& senior_email, const std::string& repo_name, const std::string& repo_tag);

   // enrolla un usuario en un proyecto
   json make_enroll_user_payload(const std::string& approver_email, const std::string& approver_password, const std::string& project_name, const std::string& target_email);

   // Desprotege un repositorio
   json make_decrypt_repo_payload(const std::string &repo_name, const std::string &email, const std::string &hashed_password);

   // Agrega un archivo a un usuario en un repositorio
   nlohmann::json make_grant_access_file_payload(const std::string& approver_email, const std::string& hashed_password,const std::string& repo_name,const std::string& file_path, const std::string& target_user_email);

   // Muestra la lista de repositorios cifrados para un usuario
   json make_list_encrypted_payload(const std::string &email, const std::string &password);

   // Muestra la lista de archivos en un repositorio accesibles para un usuario
   json make_list_accessible_payload(const std::string &email, const std::string &password);

   // Muestra la lista de archivos en un repositorio
   json make_list_files_payload(const std::string &email, const std::string &password, const std::string &repo_name);

   json make_get_key_payload(const std::string& email, const std::string& hashed_password, const std::string& repo_name, const std::string& repo_alias);
   
}
