#include "client/json_codec.hpp"

namespace client::json_nlohmann
{

    json make_nuser_payload(const std::string &name, const std::string &email, const std::string& password) {
        return json{
                {"name", name},
                {"email", email},
                {"password", password}
        };
    }

   json make_init_payload(const std::string &repo_name, const std::string &owner_email, const std::string &hashed_password) {
        return json{
            {"repo_name", repo_name},
            {"owner_email", owner_email},
            {"owner_password", hashed_password}
        };
    }

   json make_keygen_ecdsa_payload(const std::string &kPubECDSA, const std::string &email, const std::string &password) {
      return json {
                     {"cmd", "keygen"},
                     {"kpub_ecdsa", kPubECDSA},
                     {"email", email},
                     {"password", password}
                  };
   }

   json make_clone_payload(const std::string &repo_name, const std::string &destination) {
      return json{
                    {"cmd", "clone"},
                    {"data", {{"repo", repo_name}, {"destination", destination}}}
                };
   }

    json make_push_check_payload(const std::string &project_name) {
      return json{
                    {"cmd", "push_check"},
                    {"data", {{"project_name", project_name}}}
                };
    }

   json make_push_upload_payload(const std::string &project_name, const std::map<std::string, std::string>& files_map, const std::string& password) {
       json files_json;
       for (const auto& [path, hash] : files_map) files_json[path] = hash;
       
       return json{
           {"cmd", "push_upload"},
           {"data", {
               {"project_name", project_name},
               {"password", password},
               {"files_metadata", files_json}
           }}
       };
   }


   json make_log_payload(const std::string &project_name) {
       return json{
                    {"cmd", "log"},
                    {"data", {
                        {"project_name", project_name}
                    }}
                };
   }

    json make_revoke_payload(const std::string &project_name, const std::string &email, const std::string &file) {
       json j = {
                {"cmd", "revoke"},
                {"data", {
                    {"project_name", project_name},
                    {"email", email} 
                }}
            };

       if (!file.empty()) {
           j["data"]["file"] = file;
           j["data"]["scope"] = "single_file"; 
       } else {
           j["data"]["scope"] = "full_project"; 
       }

       return j;
   }

   json make_grant_payload(const std::string &project_name, const std::string &email, const std::string &file) {
       json j = {
                {"cmd", "grant"}, // Comando para el servidor
                {"data", {
                    {"project_name", project_name},
                    {"email", email}
                }}
            };

       if (!file.empty()) {
           j["data"]["file"] = file;
           j["data"]["scope"] = "single_file"; 
       } else {
           j["data"]["scope"] = "full_project"; 
       }

       return j;
   }

   json make_drop_payload(const std::string &email) {
       return json{
                {"cmd", "drop"},
                {"data", {
                    {"email", email}
                }}
            };
   }

   json make_active_payload(const std::string &email) {
       return json{
                {"cmd", "active"},
                {"data", {
                    {"email", email}
                }}
            };
   }

   json make_remove_payload(const std::string &project_name) {
       return json{
                {"cmd", "remove"},
                {"data", {
                    {"project_name", project_name}
                }}
            };
   }

   json make_verify_user_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email) {
       return json{
           {"approver_email", approver_email},
           {"approver_password", hashed_password},
           {"target_user_email", target_email}
       };
   }

   json make_change_role_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email, int new_role) {
       return json{
           {"approver_email", approver_email},
           {"approver_password", hashed_password},
           {"target_user_email", target_email},
           {"new_role", new_role}
       };
   }

   json make_change_status_payload(const std::string &approver_email, const std::string &hashed_password, const std::string &target_email, int new_status) {
       return json{
           {"approver_email", approver_email},
           {"approver_password", hashed_password},
           {"target_user_email", target_email},
           {"new_status", new_status}
       };
   }

   json make_keygen_rsa_payload(const std::string &public_key, const std::string &email, const std::string &hashed_password) {
       return json{
           {"email", email},
           {"password", hashed_password},
           {"kpub_rsa", public_key} // La clave que espera el servidor
       };
   }

   json make_cypher_repo_payload(const std::string& leader_email, const std::string& leader_password, const std::string& senior_email, const std::string& repo_name, const std::string& repo_tag) {
       return json{
           {"leader_email", leader_email},
           {"leader_password", leader_password}, // Recuerda hashear antes en el orquestador
           {"senior_email", senior_email},
           {"repo_name", repo_name},
           {"repo_tag", repo_tag}
       };
   }

   json make_enroll_payload(const std::string& approver_email, const std::string& approver_password, const std::string& project_name, const std::string& target_email) {
       return json{
           {"approverEmail", approver_email},
           {"approverPassword", approver_password}, // Recuerda hashear antes
           {"projectName", project_name},
           {"userEmail", target_email}
       };
   }

} // namespace client::json_nlohmann