#pragma once
#include <string>
#include <vector>

namespace client::cmd {

   bool run_config(const std::string& name, const std::string& email);

   bool run_init(const std::string&, const std::vector<std::string>&);

   bool run_keygen(const std::string& keyType, const std::string &pathToOutput, const std::string& email, const std::string& password);

   bool run_clone(const std::string&, const std::string&);
  
   bool run_push(const std::string&, const std::string&);

   bool run_log(const std::string& project_name);

   bool run_revoke(const std::string& project_name, const std::string& email, const std::string& file_target = "");

   bool run_grant(const std::string& project_name, const std::string& email, const std::string& file_target = "");

   bool run_drop(const std::string& email);

   bool run_active(const std::string& email);

   bool run_remove(const std::string& project_name);

   
}