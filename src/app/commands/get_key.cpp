#include "client/colors.hpp"
#include "client/commands.hpp"
#include "client/key_loader.hpp"
#include "client/json_codec.hpp"
#include "client/files_codec.hpp"
#include "client/client_https.hpp"
#include "client/hasher_codec.hpp"
#include "client/response_handler.hpp"
#include "client/decipher_RSA_codec.hpp"


namespace client::cmd {

    bool run_get_key(const std::string& email, const std::string& password, const std::string& repo_name, const std::string& repo_alias, const std::string& output_dir) {
        
        std::cout << "\n" << client::colors::BOLD << client::colors::MAGENTA 
                  << "--- Retrieving Repository AES Key ---" 
                  << client::colors::RESET << std::endl;
        
        std::cout << client::colors::YELLOW << "User: " << client::colors::RESET << email << std::endl;
        std::cout << client::colors::YELLOW << "Repo: " << client::colors::RESET << repo_name << " (" << repo_alias << ")" << std::endl;

        // reponame + alias para el archivo
        std::string repo_name_with_alias = repo_name + "_" + repo_alias;

        try {
            // Hash password
            std::string hashedPass = client::hasher_codec::hash_sha256(password);
            
            // Request from Server
            auto payload = client::json_nlohmann::make_get_key_payload(email, hashedPass, repo_name, repo_name_with_alias);
            auto response = client::http::post_json_https("/repo/protect/get_key", payload);

            // Handler (asumiendo que también lo pintarás o dejarás estándar)
            client::response_handler::handle_get_key_response(response);

            // Logical verification to continue or abort
            if (!response.contains("status") || response["status"] != "ok") {
                return false; 
            }

            if (!response.contains("aes_rsa_key")) {
                std::cerr << client::colors::RED 
                          << "[-] Critical error: Response does not contain 'aes_rsa_key'." 
                          << client::colors::RESET << std::endl;
                return false;
            }

            std::string encryptedKeyBase64 = response["aes_rsa_key"];
            std::cout << "\n" << client::colors::GREEN 
                      << "[+] Encrypted key received successfully." 
                      << client::colors::RESET << std::endl;

            // Decrypt Locally
            std::cout << client::colors::YELLOW << "    To use it, your RSA PRIVATE key is required." << client::colors::RESET << std::endl;
            
            std::cout << client::colors::BLUE << "    -> Path to your RSA private key (or directory): " << client::colors::RESET;
            std::string keyInputPath;
            std::cin >> keyInputPath;

            std::filesystem::path rsaKeyPath(keyInputPath);
            if (std::filesystem::is_directory(rsaKeyPath)) {
                rsaKeyPath /= "private_rsa.key";
            }

            if (!std::filesystem::exists(rsaKeyPath)) {
                std::cerr << "\n" << client::colors::RED 
                          << "[-] Error: Private key file not found: " << rsaKeyPath 
                          << client::colors::RESET << std::endl;
                return false;
            }

            // Load RSA Private Key
            client::key_loader::RSAPrivateKey rsaPrivKey;
            if (!client::key_loader::load_private_key(rsaKeyPath.string(), rsaPrivKey)) {
                std::cerr << "\n" << client::colors::RED 
                          << "[-] Critical error: RSA private key is invalid or corrupt." 
                          << client::colors::RESET << std::endl;
                return false;
            }

            // Decrypt (RSA-OAEP)
            std::string aesKeyRaw;
            if (!client::decipher_RSA_codec::OAEP_decryptFile(rsaPrivKey, encryptedKeyBase64, aesKeyRaw)) {
                 std::cerr << "\n" << client::colors::RED 
                           << "[-] RSA decryption failed. Verify it is the correct key." 
                           << client::colors::RESET << std::endl;
                 return false;
            }

            // Save
            std::filesystem::path outPath(output_dir);
            if (std::filesystem::is_directory(outPath)) {
                // Format: RepoName_Alias_AES.key
                std::string filename =  repo_name_with_alias + "_AES.key";
                outPath /= filename;
            }

            if (client::files_codec::save_string_to_file(aesKeyRaw, outPath.string())) {
                std::cout << "\n" << client::colors::GREEN 
                          << "[+] AES key saved to: " << outPath.string() 
                          << client::colors::RESET << std::endl;
                std::cout << "         You can now use 'orca uncyp' with this file." << std::endl;
                return true;
            } else {
                std::cerr << "\n" << client::colors::RED 
                          << "[-] Error writing file to disk." 
                          << client::colors::RESET << std::endl;
                return false;
            }

        } catch (const std::exception& e) {
            std::cerr << "\n" << client::colors::RED 
                      << "[!] Exception: " << e.what() 
                      << client::colors::RESET << std::endl;
            return false;
        }
    }
}