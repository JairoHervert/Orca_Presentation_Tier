#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/key_loader.hpp"
#include "client/files_codec.hpp"
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp"
#include "client/response_handler.hpp"
#include "client/decipher_RSA_codec.hpp"

namespace client::cmd {

    bool run_encrypt_repo(const std::string& leader_email, const std::string& leader_password, 
                         const std::string& senior_email, const std::string& repo_name, 
                         const std::string& repo_tag) { 

        std::cout << "\n --- Protecting Repository (Encryption) ---" << std::endl;
        std::cout << "Project: " << repo_name << " (" << repo_tag << ")" << std::endl;
        std::cout << "Leader:  " << leader_email << std::endl;

        try {
            

            // Hashear password del líder
            std::string hashedPassword = client::hasher_codec::hash_sha256(leader_password);
            if (hashedPassword.empty()) {
                std::cerr << "\n[!] Internal error processing password." << std::endl;
                return false;
            }

            nlohmann::json payload = client::json_nlohmann::make_encrypt_repo_payload(leader_email, hashedPassword, senior_email, repo_name, repo_tag);

            nlohmann::json response = client::http::post_json_https("/repo/protect", payload);

            client::response_handler::handle_encrypt_repo_response(response);

            // Verificamos si el servidor mandó la clave cifrada
            if (response.contains("aes_rsa_key") && response.contains("status") && response["status"] == "ok") {
                
                std::string encryptedKey = response["aes_rsa_key"];

                // CAMBIO:
                std::cout << "\n[!] Do you want to save this key to a file now [y/n]: ";
                char opt;
                std::cin >> opt;

                if (opt == 'y' || opt == 'Y') {
                    // Pedimos la ruta de la clave privada RSA
                    std::cout << "     -> Enter the path to your RSA private key (or directory): ";
                    std::string pathKey;
                    std::cin >> pathKey;

                    std::filesystem::path keyPath(pathKey);
                    if (std::filesystem::is_directory(keyPath)) {
                        keyPath /= "private_rsa.key";
                    }

                    // Verificamos que el archivo exista
                    if (!std::filesystem::exists(keyPath)) {
                        std::cerr << "\n[-] Error: Private key file not found at: " << keyPath.string() << std::endl;
                        return false;
                    }

                    // Recuramos la clave RSA privada 
                    client::decipher_RSA_codec::RSAPrivateKey RSAprivatekey;


                    if (!client::key_loader::load_private_key(keyPath.string(), RSAprivatekey)) {
                        std::cerr << "\n[-] Critical failure: Could not load private key." << std::endl;
                        return false; 
                    }

                    std::string AESkey;
                    // Intentamos descifrar
                    if(!client::decipher_RSA_codec::OAEP_decryptFile(RSAprivatekey, encryptedKey, AESkey)) {
                         std::cerr << "\n[-] Decryption failed. Verify it is the correct key." << std::endl;
                         return false;
                    }

                    //ingresar ruta y nombre de archivo para guardar la clave AES
                    std::cout << "     -> Enter the path to save the AES key: ";
                    std::string filename;
                    std::cin >> filename;
                    //agregar el _tag al AES.key
                    //filename += "_" + repo_tag + "\\AES.key";
                    // con tag y nombre de repo
                    filename += "\\" + repo_name + "_" + repo_tag + "_AES.key";
                    if (client::files_codec::save_string_to_file(AESkey, filename)) {
                        std::cout << "\n[+] Key saved successfully to: " << filename << std::endl;
                        std::cout << "    Keep it in a safe place." << std::endl;
                    } else {
                        std::cerr << "\n[-] Error: Could not save the file." << std::endl;
                        return false;
                    }

                } else {
                    std::cout << "\n[!] Operation finished without saving the key locally." << std::endl;
                    return false;
                }
            }

            return true;

        } catch (const std::exception &e) {
            std::cerr << "\n[-] Error in encrypt_repo: " << e.what() << std::endl;
            return false;
        }
    }
}