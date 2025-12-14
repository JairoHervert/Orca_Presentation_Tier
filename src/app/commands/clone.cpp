#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp" 
#include "client/unpacker_codec.hpp" 
#include "client/response_handler.hpp"
#include "client/ensure_directory.hpp"

namespace client::cmd {


    bool run_clone(const std::string& repo_name, const std::string& destination, const std::string& email, const std::string& password) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "--- Cloning Repository ---" 
                  << client::colors::RESET << std::endl;
        std::cout << client::colors::YELLOW << "Project:     " << client::colors::RESET << repo_name << std::endl;
        std::cout << client::colors::YELLOW << "Destination: " << client::colors::RESET << destination << std::endl;

        try {
            // Asegurar que el directorio de destino exista
            if (!client::ensure_directory::ensure_directory(destination)) {
                std::cerr << "\n" << client::colors::RED 
                          << "[-] Error: Destination path is invalid or cannot be created." 
                          << client::colors::RESET << std::endl;
                return false;
            }

            // Hashear password
            std::string hashedPassword = client::hasher_codec::hash_sha256(password);
            if (hashedPassword.empty()) {
                std::cerr << "\n" << client::colors::RED 
                          << "[-] Internal security error." 
                          << client::colors::RESET << std::endl;
                return false;
            }

            // Payload
            auto payload = client::json_nlohmann::make_clone_payload(repo_name, email, hashedPassword);

            // Descargar
            // Nota: Aquí el propio downloader podría imprimir progreso, pero asumimos que es silencioso o manejamos nosotros el output
            nlohmann::json response = client::http::post_download_file("/repo/clone", payload, repo_name + ".tar");

            client::response_handler::handle_clone_response(response);

            if (response.value("status", "error") != "ok") {
                return false; 
            }

            if (!response.contains("downloaded_file")) {
                std::cerr << client::colors::RED 
                          << "[-] Internal error: Downloaded file not found." 
                          << client::colors::RESET << std::endl;
                return false;
            }

            std::string downloaded_file = response["downloaded_file"];
            std::string tar_abs_path = std::filesystem::absolute(downloaded_file).string();

            // PROCESO: Azul para indicar actividad
            std::cout << "    " << client::colors::BLUE 
                      << "[*] Processing files (Unpacking)..." 
                      << client::colors::RESET << std::endl;

            // Desempaquetar
            if (client::unpacker::unpack_file(tar_abs_path, destination)) {
                
                // Limpiar
                try {
                    if (std::filesystem::exists(tar_abs_path)) std::filesystem::remove(tar_abs_path);
                } catch (...) {}

                std::cout << client::colors::MAGENTA << "--------------------------------------" << client::colors::RESET << std::endl;
                
                std::cout << client::colors::GREEN 
                          << "[+] Repository cloned successfully." 
                          << client::colors::RESET << std::endl;
                
                std::cout << "    Your files are ready in: " << client::colors::BOLD << destination << client::colors::RESET << std::endl;
                return true;

            } else {
                
                std::cerr << client::colors::MAGENTA << "--------------------------------------" << client::colors::RESET << std::endl;
                
                std::cerr << client::colors::RED 
                          << "[-] Critical error: Repository downloaded but extraction failed." 
                          << client::colors::RESET << std::endl;
                
                std::cerr << client::colors::RED 
                          << "    The corrupted file is at: " << tar_abs_path 
                          << client::colors::RESET << std::endl;
                return false;
            }

        } catch (const std::exception &e) {
            std::cerr << "\n " << client::colors::RED 
                      << "[-] Exception: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }
}