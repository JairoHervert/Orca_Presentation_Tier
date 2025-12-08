#include "../third_party/json.hpp"

#pragma once

#include <string>
#include <iomanip>
#include <iostream>
#include <algorithm>


namespace client::response_handler {

   void handle_nuser_response(const nlohmann::json &response);

   void handle_init_response(const nlohmann::json &response);

   void handle_keygen_response(const nlohmann::json &response);

   void handle_clone_response(const nlohmann::json &response);

   void handle_push_check_response(const nlohmann::json &response);

   void handle_log_response(const nlohmann::json &response);

   void handle_verify_response(const nlohmann::json &response);

   void handle_change_role_response(const nlohmann::json &response);

   void handle_change_status_response(const nlohmann::json &response);

   void handle_keygen_rsa_response(const nlohmann::json &response);

   void handle_cypher_repo_response(const nlohmann::json &response);

   void handle_enroll_response(const nlohmann::json &response);

   void handle_uncyp_response(const nlohmann::json &response);

   void handle_add_user_file_response(const nlohmann::json &response);

   void handle_commits_response(const nlohmann::json &response);

   void handle_list_repos_response(const nlohmann::json &response);

   void handle_list_encrypted_response(const nlohmann::json &response);

   void handle_list_accessible_response(const nlohmann::json &response);

   void handle_list_files_response(const nlohmann::json &response, const std::string& repo_name);
   // Otras funciones de manejo de respuestas pueden ir aquí

}