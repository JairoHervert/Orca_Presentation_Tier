#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/key_loader.hpp"        
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp" 
#include "client/unpacker_codec.hpp" 
#include "client/response_handler.hpp" 
#include "client/decipher_AES_codec.hpp" 


namespace client::cmd {

    bool run_decrypt_repo(const std::string& repo_name, const std::string& repo_tag, const std::string& destination, 
                   const std::string& email, const std::string& password, 
                   const std::string& key_path) {

        //reponame + tag para descargar el archivo correcto
        std::string repo_name_with_tag = repo_name + "_" + repo_tag;
        std::cout << "\n--- Decrypting Repository ---" << std::endl;
        std::cout << "Project: " << repo_name_with_tag << std::endl;
        std::cout << "Destination: " << destination << std::endl;

        try {

            // Ruta de clave tag_AES.key
            std::string key_file_name = repo_name_with_tag + "_AES.key";
            std::filesystem::path dirPath(key_path);
            std::filesystem::path fullKeyPath = dirPath / key_file_name;

            // Verificar que el archivo exista 
            if (!std::filesystem::exists(fullKeyPath)) {
                std::cerr << "\n[-] Error: 'AES.key' file not found in directory: " << key_path << std::endl;
                return false;
            }

            // Cargar la clave
            std::string aes_key_raw;
            if (!client::key_loader::load_aes_key(fullKeyPath.string(), aes_key_raw)) {
                return false;
            }

            // Hashear la contraseña
            std::string hashedPassword = client::hasher_codec::hash_sha256(password);
            if (hashedPassword.empty()) {
                std::cerr << "\n[-] Internal security error (Hashing)." << std::endl;
                return false;
            }

            auto payload = client::json_nlohmann::make_decrypt_repo_payload(repo_name_with_tag, email, hashedPassword);

            // Descargar el archivo cifrado (.tar.enc)
            nlohmann::json response = client::http::post_download_file("/repo/unprotect", payload, repo_name_with_tag + ".tar.enc");

            client::response_handler::handle_decrypt_repo_response(response);

            // Verificar si hubo un error 
            if (response.value("status", "error") != "ok") {
                return false;
            }

            if (!response.contains("downloaded_file")) {
                std::cerr << "\n[-] Error: Downloaded file path not found." << std::endl;
                return false;
            }

            std::string encrypted_file_path = response["downloaded_file"]; 
            std::string encrypted_abs_path = std::filesystem::absolute(encrypted_file_path).string();
            
            // Calculamos el nombre del archivo temporal descifrado (.tar)
            std::string tar_file_path = encrypted_abs_path;
            if (tar_file_path.substr(tar_file_path.find_last_of(".") + 1) == "enc") {
                tar_file_path = tar_file_path.substr(0, tar_file_path.find_last_of("."));
            } else {
                tar_file_path += ".tar"; 
            }

            // Descifrar con AES-GCM
            std::cout << "\n    Decrypting data (Using local key)..." << std::endl;

            // Pasamos 'aes_key_raw' que contiene los bytes listos para usar
            if (!client::decipher_aes::decipher_AES_GCM(encrypted_abs_path, tar_file_path, aes_key_raw)) {
                std::cerr << "[-] Decryption failed." << std::endl; 
                
                // Limpieza parcial
                try { std::filesystem::remove(encrypted_abs_path); } catch(...) {}
                return false;
            }

            // Desempaquetar
            std::cout << "    Extracting files..." << std::endl;
            
            std::filesystem::path baseDest(destination);
            std::filesystem::path finalDest = baseDest / repo_name_with_tag;

            // Pasamos la ruta nueva al unpacker
            if (client::unpacker::unpack_file(tar_file_path, finalDest.string())) {
                
                // Limpieza
                try {
                    if (std::filesystem::exists(encrypted_abs_path)) std::filesystem::remove(encrypted_abs_path);
                    if (std::filesystem::exists(tar_file_path)) std::filesystem::remove(tar_file_path);
                } catch (...) {}

                std::cout << "--------------------------------------" << std::endl;
                std::cout << "[+] Repository recovered successfully." << std::endl;
                std::cout << "     Final location: " << finalDest.string() << std::endl; 
                return true;

            } else {
                std::cerr << "[-] Critical error unpacking the file." << std::endl;
                try { 
                    if (std::filesystem::exists(encrypted_abs_path)) std::filesystem::remove(encrypted_abs_path);
                    if (std::filesystem::exists(tar_file_path)) std::filesystem::remove(tar_file_path);
                } catch(...) {}
                return false;
            }

        } catch (const std::exception &e) {
            std::cerr << "[-] Critical exception in run_decrypt_repo: " << e.what() << std::endl;
            return false;
        }
    }
}