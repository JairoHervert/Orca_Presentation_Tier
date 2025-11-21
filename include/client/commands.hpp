#pragma once
#include <string>
#include <vector>

namespace client::cmd {

   bool run_init(const std::string&, const std::vector<std::string>&);

   bool run_clone(const std::string&, const std::string&);
  
   bool run_push(const std::string&, const std::string&);

   bool run_config(const std::string& name, const std::string& email);

   bool run_log(const std::string& project_name);

   // Revoca los permisos de un usuario.
   // Si file_target esta vacio, revoca todo el proyecto.
   bool run_revoke(const std::string& project_name, const std::string& user_target, const std::string& file_target = "");


   
}