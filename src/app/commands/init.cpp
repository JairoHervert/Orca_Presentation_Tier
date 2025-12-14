#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/hasher_codec.hpp" 
#include "client/client_https.hpp"
#include "client/response_handler.hpp"


namespace client::cmd {

   bool run_init(const std::string& repo_name, const std::string& email, const std::string& password) {
      
      std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                << " --- Initializing New Repository ---" 
                << client::colors::RESET << std::endl;
      
      std::cout << client::colors::YELLOW << "Name:  " << client::colors::RESET << repo_name << std::endl;
      std::cout << client::colors::YELLOW << "Owner: " << client::colors::RESET << email << std::endl;

      // Hashear password
      std::string hashedPassword = client::hasher_codec::hash_sha256(password);
      if (hashedPassword.empty()) {
          std::cerr << client::colors::RED << "[-] Error processing password." << client::colors::RESET << std::endl;
          return false;
      }

      // Crear Payload
      nlohmann::json payload = client::json_nlohmann::make_init_payload(repo_name, email, hashedPassword);

      std::cout<< std::endl;

      try {
          nlohmann::json response = client::http::post_json_https("/repo/init", payload);

          // El handler maneja la respuesta (puedes actualizarlo también para client::colorsores)
          client::response_handler::handle_init_response(response);
          
          if (response.contains("status") && response["status"] == "ok") {
              return true;
          }

      } catch (const std::exception &e) {
          std::cerr << "\n" << client::colors::RED 
                    << "[!] Connection Error: " << e.what() 
                    << client::colors::RESET << std::endl;
          return false;
      }

      return false;
   }
}