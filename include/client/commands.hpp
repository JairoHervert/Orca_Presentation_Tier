#pragma once
#include <string>
#include <vector>

namespace client::cmd {

   bool run_init(const std::string&, const std::vector<std::string>&);

   bool run_clone(const std::string&, const std::string&);

   
}