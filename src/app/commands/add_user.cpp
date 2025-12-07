#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp"

namespace client::cmd {

    bool run_add_user_file(const std::string& repo_name,  const std::string& file_path, const std::string& target_email, const std::string& senior_email,  const std::string& password) {
        std::cout << "\n--- Granting File Access ---" << std::endl;
        std::cout << "Project: " << repo_name << std::endl;
        std::cout << "File:    " << file_path << std::endl;
        std::cout << "User:    " << target_email << std::endl;

        try {
            // 1. Hashear password del Senior/Approver
            std::string hashedPassword = client::hasher_codec::hash_sha256(password);
            if (hashedPassword.empty()) {
                std::cerr << "\n[-] Internal security error." << std::endl;
                return false;
            }

            // 2. Crear Payload
            auto payload = client::json_nlohmann::make_add_user_file_payload(senior_email, hashedPassword, repo_name, file_path, target_email);

            // 3. Enviar POST
            nlohmann::json response = client::http::post_json_https("/repo/file/add_user", payload);

            // 4. Manejar Respuesta
            client::response_handler::handle_add_user_file_response(response);

            // 5. Retornar éxito si status es ok
            return (response.value("status", "error") == "ok");

        } catch (const std::exception &e) {
            std::cerr << "\n[-] Exception: " << e.what() << std::endl;
            return false;
        }
    }
}