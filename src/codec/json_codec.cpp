#include "client/json_codec.hpp"

namespace client::json_nlohmann
{

   json make_init_payload(const std::string &repo_name, const std::vector<std::string> &collaborators) {
      return json {
                     {"cmd", "init"},
                     {"data", {{"name", repo_name}, {"collaborators", collaborators}}}};
   }

   json make_clone_payload(const std::string &repo_name, const std::string &destination) {
      return json{
         {"cmd", "clone"},
         {"data", {{"repo", repo_name}, {"destination", destination}}}};
   }

    json make_push_check_payload(const std::string &project_name) {
      return json{
         {"cmd", "push_check"},
         {"data", {{"project_name", project_name}}}};
   }
   json make_push_upload_payload(const std::string &project_name, const std::map<std::string, std::string>& files_map) {
       json files_json;
       for (const auto& [path, hash] : files_map) files_json[path] = hash;
       return json{
         {"cmd", "push_upload"}, 
         {"data", {{"project_name", project_name}, 
         {"files_metadata", files_json}}}};
   }

   json make_config_payload(const std::string &name, const std::string &email) {
       return json{
           {"cmd", "config"},
           {"data", {
               {"name", name},
               {"email", email}
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

} // namespace client::json_nlohmann