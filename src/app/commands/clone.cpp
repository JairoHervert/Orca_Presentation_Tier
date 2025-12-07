
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp" 
#include "client/unpacker_codec.hpp" 
#include "client/response_handler.hpp"
#include "client/ensure_directory.hpp"
namespace client::cmd {

    bool run_clone(const std::string& repo_name, const std::string& destination, const std::string& email, const std::string& password) {
        std::cout << "\n--- Cloning Repository ---" << std::endl;
        std::cout << "Project: " << repo_name << std::endl;
        std::cout << "Destination: " << destination << std::endl;

        try {
            // Asegurar que el directorio de destino exista
            if (!client::ensure_directory::ensure_directory(destination)) {
                std::cerr << "\n[-] Error: Destination path is invalid or cannot be created." << std::endl;
                return false;
            }

            // Hashear password
            std::string hashedPassword = client::hasher_codec::hash_sha256(password);
            if (hashedPassword.empty()) {
                std::cerr << "\n[-] Internal security error." << std::endl;
                return false;
            }

            // Payload
            auto payload = client::json_nlohmann::make_clone_payload(repo_name, email, hashedPassword);

            // Descargar
            nlohmann::json response = client::http::post_download_file("/repo/clone", payload, repo_name + ".tar");

            // Mostrar respuesta del servidor
            client::response_handler::handle_clone_response(response);

            if (response.value("status", "error") != "ok") {
                return false; 
            }

            if (!response.contains("downloaded_file")) {
                std::cerr << "[-] Internal error: Downloaded file not found." << std::endl;
                return false;
            }

            std::string downloaded_file = response["downloaded_file"];
            std::string tar_abs_path = std::filesystem::absolute(downloaded_file).string();

            std::cout << "    [*] Processing files (Unpacking)..." << std::endl;



            // Desempaquetar
            if (client::unpacker::unpack_file(tar_abs_path, destination)) {
                
                // Limpiar
                try {
                    if (std::filesystem::exists(tar_abs_path)) std::filesystem::remove(tar_abs_path);
                } catch (...) {}

                std::cout << "--------------------------------------" << std::endl;
                std::cout << "[+] Repository cloned successfully." << std::endl;
                std::cout << "     Your files are ready in: " << destination << std::endl;
                return true;

            } else {
                
                std::cerr << "--------------------------------------" << std::endl;
                std::cerr << "[-] Critical error: Repository downloaded but extraction failed." << std::endl;
                std::cerr << "    The corrupted file is at: " << tar_abs_path << std::endl;
                return false;
            }

        } catch (const std::exception &e) {
            std::cerr << "\n [-] Exception: " << e.what() << std::endl;
            return false;
        }
    }
}