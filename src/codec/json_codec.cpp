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

   json make_clone_payload(const std::string &repo_name, const std::string &email, const std::string &hashed_password) {
       return json{
           {"repoName", repo_name},
           {"userEmail", email},
           {"userPassword", hashed_password} 
       };
   }


    json make_push_check_payload(const std::string& repo_name, const std::string& email, const std::string& hashed_password) {
        return json{
            {"repoName", repo_name},
            {"userEmail", email},
            {"userPassword", hashed_password}
        };
    }

    json make_push_upload_payload(const std::string& repo_name, const std::string& email, const std::string& hashed_password, const std::map<std::string, std::string>& signatures) {
        return json{
            {"repoName", repo_name},
            {"userEmail", email},
            {"userPassword", hashed_password},
            {"files_signatures", signatures} // Mapa clave-valor con las firmas
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

   nlohmann::json make_unprotect_payload(const std::string &repo_name, const std::string &email, const std::string &hashed_password) {
        return nlohmann::json{
            {"repoName", repo_name},
            {"userEmail", email},
            {"userPassword", hashed_password} 
        };
    }

} // namespace client::json_nlohmann