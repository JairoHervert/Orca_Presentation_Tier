#pragma once
#include <string>
#include <vector>

namespace client::cmd {

   bool run_config(const std::string&, const std::string&);

   bool run_init(const std::string&, const std::vector<std::string>&);

   bool run_keygen(const std::string&, const std::string&, const std::string&, const std::string&);

   bool run_clone(const std::string&, const std::string&);
  
   bool run_push(const std::string&, const std::string&, const std::string&, const std::string&);

   bool run_log(const std::string&);

   bool run_revoke(const std::string&, const std::string&, const std::string&);

   bool run_grant(const std::string&, const std::string&, const std::string&);

   bool run_drop(const std::string&);

   bool run_active(const std::string&);

   bool run_remove(const std::string&);

   
}