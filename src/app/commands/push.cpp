#include "client/commands.hpp"
#include "client/sign_codec.hpp" 
#include "client/json_codec.hpp"
#include "client/key_loader.hpp"
#include "client/hasher_codec.hpp"
#include "client/client_https.hpp"
#include "client/packer_codec.hpp"
#include "client/scanner_codec.hpp" 
#include "client/comparator_codec.hpp"

namespace client::cmd {

    bool run_push(const std::string& project_name, const std::string& email, const std::string& directory, const std::string& key_path, const std::string& password) {
        std::cout << "\n--- Starting Push: " << project_name << " ---" << std::endl;
        
        std::filesystem::path base_path(directory);
        std::filesystem::path repo_path = base_path / project_name;
        
        // 1. Validations and Key Loading
        if (!std::filesystem::exists(repo_path)) return false;
        
        std::filesystem::path privateKeyPath = std::filesystem::path(key_path) / "private_ecdsa.key";
        client::key_loader::ECDSAPrivateKey privateKey;
        if (!client::key_loader::load_private_key(privateKeyPath.string(), privateKey)) return false;

        try {
            std::string hashedPass = client::hasher_codec::hash_sha256(password);

            // pedir los hash al server
            auto payload_check = client::json_nlohmann::make_push_check_payload(project_name, email, hashedPass);
            auto response_check = client::http::post_json_https("/push/check", payload_check);
            
            if (!response_check.contains("status") || response_check["status"] != "success") {
                std::cerr << "\n[!] Server check error." << std::endl;
                return false;
            }

            std::map<std::string, std::string> remote_files;
            if(response_check.contains("server_hashes")) {
                remote_files = response_check["server_hashes"].get<std::map<std::string, std::string>>();
            }

            // Compare local vs remote
            std::cout << "[*] Calculating changes..." << std::endl;
            auto local_files = client::scanner::generate_file_map(repo_path.string());
            auto diff = client::comparator::compute_diff(local_files, remote_files);

            if (diff.to_upload.empty() && diff.to_delete.empty()) {
                std::cout << "[+] Repository up to date. No changes." << std::endl;
                return true;
            }

            // 4. PREPARAR OPERACIONES
            std::vector<client::json_nlohmann::PushOperation> operations;
            std::cout << "[3] Firmando cambios..." << std::endl;

            // A. Procesar UPDATES (Nuevos y Modificados)
            for (const auto& rel_path : diff.to_upload) {
                // OPTIMIZACIÓN: Ya no necesitamos la ruta absoluta para leer el archivo.
                // Usamos el hash que YA calculamos en el scanner.
                
                std::string fileHash = local_files[rel_path]; // <--- ¡AQUÍ ESTÁ EL TRUCO!
                std::string signature;
                
                // Pasamos el hash directamente
                if (client::sign_codec::sign_file_for_update(privateKey, fileHash, signature)) {
                    operations.push_back({"update", rel_path, signature});
                } else {
                    std::cerr << "[-] Error firmando: " << rel_path << std::endl;
                    return false;
                }
            }

            // B. Process DELETES (Sign server's base64 hash)
            for (const auto& rel_path : diff.to_delete) {
                std::string serverHash = remote_files[rel_path]; // Get the hash sent by server
                std::string signature;

                // Use specific function for Deletes (Base64 String -> Signature)
                if (client::sign_codec::sign_hash_string_for_delete(privateKey, serverHash, signature)) {
                    operations.push_back({"delete", rel_path, signature});
                } else {
                    std::cerr << "[-] Error signing deletion: " << rel_path << std::endl;
                    return false;
                }
            }

            // 5. PACK (Only uploads)
            std::string temp_tar = "push_upload.tar.gz";
            
            // Create empty tar if no uploads (only deletes), or fill with uploads
            if (!diff.to_upload.empty()) {
                auto original_path = std::filesystem::current_path();
                std::filesystem::current_path(repo_path);
                client::packer::pack_files(diff.to_upload, temp_tar); 
                
                std::string tar_abs_path = (original_path / temp_tar).string();
                std::filesystem::rename(temp_tar, tar_abs_path);
                std::filesystem::current_path(original_path);
            } else {
                // Create dummy empty file to prevent read errors
                std::ofstream(temp_tar).close();
            }
            
            std::string tar_abs_path = std::filesystem::absolute(temp_tar).string();

            // 6. UPLOAD (v2)
            std::cout << "[4] Sending to server..." << std::endl;
            
            // Convert operations vector to JSON string
            std::string opsJson = client::json_nlohmann::make_push_operations_json(operations);

            auto res_upload = client::http::upload_push_data(
                "/repo/push/upload", 
                project_name, 
                email, 
                hashedPass, 
                opsJson, 
                tar_abs_path
            );

            // Cleanup
            if (std::filesystem::exists(tar_abs_path)) std::filesystem::remove(tar_abs_path);

            if (res_upload.contains("status") && res_upload["status"] == "success") {
                std::cout << "\n[SUCCESS] " << res_upload.value("message", "Push completed.") << std::endl;
                return true;
            } else {
                std::cerr << "\n[-] Error: " << res_upload.value("message", "Unknown failure") << std::endl;
                return false;
            }

        } catch (const std::exception &e) {
            std::cerr << "[!] Exception: " << e.what() << std::endl;
            return false;
        }
    }
}