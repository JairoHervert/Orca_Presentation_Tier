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

} // namespace client::json_nlohmann
