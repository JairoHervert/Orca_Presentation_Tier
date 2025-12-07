#pragma once
#include <string>
#include <vector>
#include "../third_party/json.hpp"

namespace client::json_nlohmann {

   using json = nlohmann::json;

   // Crea el payload para el comando 'nuser' 
   json make_nuser_payload(const std::string &name, const std::string &email, const std::string& password);
   
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
   json make_change_role_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email, int new_role);

   // Cambia el status de un usuario
   json make_change_status_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email, int new_status);
   
   // Generar Claves RSA_OAEP
   json make_keygen_rsa_payload(const std::string &public_key, const std::string &email, const std::string &hashed_password);

   // Cifra un repositorio 
   json make_cypher_repo_payload(const std::string& leader_email, const std::string& leader_password, const std::string& senior_email, const std::string& repo_name, const std::string& repo_tag);

   // Enrolla un usuario en un proyecto
   json make_enroll_payload(const std::string& approver_email, const std::string& approver_password, const std::string& project_name, const std::string& target_email);

   // Desprotege un repositorio
   json make_unprotect_payload(const std::string &repo_name, const std::string &email, const std::string &hashed_password);

   // Agrega un archivo a un usuario en un repositorio
   nlohmann::json make_add_user_file_payload(const std::string& approver_email, const std::string& hashed_password,const std::string& repo_name,const std::string& file_path, const std::string& target_user_email);
}
