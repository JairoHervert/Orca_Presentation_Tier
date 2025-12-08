#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/hasher_codec.hpp" 
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
namespace client::cmd {

    bool run_set_status(const std::string& approver_email, const std::string& approver_pass, const std::string& target_email, int new_status) {
        std::cout << "\n --- Changing User Status ---" << std::endl;
        std::cout << "Approver: " << approver_email << std::endl;
        std::cout << "Target:   " << target_email << std::endl;
        std::cout << "New Status: " << new_status << " (0=Inactive, 1=Active)" << std::endl;

        if (new_status < 0 || new_status > 1) {
            std::cerr << "\n[-] Error: Status must be 0 or 1." << std::endl;
            return false;
        }
        
        try {
            // Hash-password
            std::string hashedPassword = client::hasher_codec::hash_sha256(approver_pass);

            // Crear Payload
            nlohmann::json payload = client::json_nlohmann::make_set_status_payload(approver_email, hashedPassword, target_email, new_status);

            nlohmann::json response = client::http::post_json_https("/user/change_status", payload);

            // Manejar respuesta
            client::response_handler::handle_set_status_response(response);
            
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }
}