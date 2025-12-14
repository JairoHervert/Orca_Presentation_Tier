#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp" 
#include "client/response_handler.hpp"


namespace client::cmd {

    bool run_create_user(const std::string& name, const std::string& email, const std::string& password) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "--- Registering New User ---" 
                  << client::colors::RESET << std::endl;
        
        std::cout << client::colors::YELLOW << "User:  " << client::colors::RESET << name << std::endl;
        std::cout << client::colors::YELLOW << "Email: " << client::colors::RESET << email << std::endl;
 
        try {
            std::string hashedPassword = client::hasher_codec::hash_sha256(password);
            
            if (hashedPassword.empty()) {
                std::cerr << client::colors::RED << "[-] Error hashing the password." << client::colors::RESET << std::endl;
                return false;
            }
            
            // Creamos el payload 
            nlohmann::json payload = client::json_nlohmann::make_create_user_payload(name, email, hashedPassword);

            // Enviamos la petición
            std::cout << std::endl;
            nlohmann::json response = client::http::post_json_https("/user/create", payload);

            client::response_handler::handle_create_user_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << client::colors::RED << "[-] Error registering user: " << e.what() << client::colors::RESET << std::endl;
            return false;
        }
    }
}