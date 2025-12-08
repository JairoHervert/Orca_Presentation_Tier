#include "client/commands.hpp"
#include "client/sign_codec.hpp" 
#include "client/json_codec.hpp"
#include "client/key_loader.hpp"
#include "client/hasher_codec.hpp"
#include "client/client_https.hpp"
#include "client/packer_codec.hpp"
#include "client/scanner_codec.hpp" 
#include "client/comparator_codec.hpp"
#include "client/response_handler.hpp"

namespace client::cmd {

    bool run_push(const std::string& project_name, const std::string& email, const std::string& directory, const std::string& key_path, const std::string& password) {
        std::cout << "\n--- Starting Push: " << project_name << " ---" << std::endl;
        
        std::filesystem::path base_path(directory);
        std::filesystem::path repo_path = base_path / project_name;
        
        // 1. Validations
        if (!std::filesystem::exists(repo_path)) {
             std::cerr << "[-] Error: Project folder does not exist." << std::endl; 
             return false; 
        }
        
        std::filesystem::path privateKeyPath = std::filesystem::path(key_path) / "private_ecdsa.key";
        client::key_loader::ECDSAPrivateKey privateKey;
        if (!client::key_loader::load_private_key(privateKeyPath.string(), privateKey)) {
            std::cerr << "[-] Error: Invalid private key." << std::endl;
            return false;
        }

        try {
            std::string hashedPass = client::hasher_codec::hash_sha256(password);

            auto payload_check = client::json_nlohmann::make_push_check_payload(project_name, email, hashedPass);
            auto response_check = client::http::post_json_https("/repo/push/hash", payload_check); // Correct endpoint
            
            client::response_handler::handle_push_check_response(response_check);

            if (!response_check.contains("status") || (response_check["status"] != "success" && response_check["status"] != "ok")) {
                return false;
            }

            std::map<std::string, std::string> remote_files;
            if(response_check.contains("files")) {
                auto raw_map = response_check["files"].get<std::map<std::string, std::string>>();
                for (const auto& [path, hash] : raw_map) {
                    std::string clean_path = path;
                    std::replace(clean_path.begin(), clean_path.end(), '\\', '/');
                    
                    remote_files[clean_path] = hash;
                }
                // ------------------------------------------
            }

            // 3. SCAN & DIFF (Phase 2)
            std::cout << "[+] Calculating changes..." << std::endl;
            auto local_files = client::scanner::generate_file_map(repo_path.string());
            auto diff = client::comparator::compute_diff(local_files, remote_files);

            if (diff.to_upload.empty() && diff.to_delete.empty()) {
                std::cout << "[+] Repository up to date. No changes." << std::endl;
                return true;
            }

            // 4. PREPARE OPERATIONS
            std::vector<client::json_nlohmann::PushOperation> operations;

            // A. UPDATES (Sign Binary Hash)
            for (const auto& rel_path : diff.to_upload) {
                // Get the hash we already calculated in scanner
                std::string fileHash = local_files[rel_path]; 
                std::string signature;
                
                if (client::sign_codec::sign_file_for_update(privateKey, fileHash, signature)) {
                    operations.push_back({"update", rel_path, signature});
                } else {
                    std::cerr << "\n[-] Error signing: " << rel_path << std::endl;
                    return false;
                }
            }

            // B. DELETES (Sign Server Base64 Hash)
            for (const auto& rel_path : diff.to_delete) {
                std::string serverHash = remote_files[rel_path];
                std::string signature;

                if (client::sign_codec::sign_hash_string_for_delete(privateKey, serverHash, signature)) {
                    operations.push_back({"delete", rel_path, signature});
                } else {
                    std::cerr << "\n[-] Error signing deletion: " << rel_path << std::endl;
                    return false;
                }
            }

            // 5. PACK (Only uploads)
            std::string temp_tar = "push_upload.tar.gz";
            
            if (!diff.to_upload.empty()) {
                auto original_path = std::filesystem::current_path();
                std::filesystem::current_path(repo_path);
                
                client::packer::pack_files(diff.to_upload, temp_tar); 
                
                std::string tar_abs_path = (original_path / temp_tar).string();
                std::filesystem::rename(temp_tar, tar_abs_path);
                std::filesystem::current_path(original_path);
            } else {
                // Create dummy if only deleting
                std::ofstream(temp_tar).close();
            }
            
            std::string tar_abs_path = std::filesystem::absolute(temp_tar).string();

            // 6. UPLOAD (Phase 4)
            std::cout << "\n[*] Sending to server..." << std::endl;
            
            std::string opsJson = client::json_nlohmann::make_push_operations_json(operations);

            // Call your upload_push_data function (ensure it uses separate Multipart)
            auto res_upload = client::http::upload_push_data("/repo/push/upload", project_name, email, hashedPass, opsJson, tar_abs_path);

            // Cleanup
            if (std::filesystem::exists(tar_abs_path)) std::filesystem::remove(tar_abs_path);

            client::response_handler::handle_push_upload_response(res_upload);

            return (res_upload.value("status", "error") == "success" || res_upload.value("status", "error") == "ok");

        } catch (const std::exception &e) {
            std::cerr << "\n[!] Exception: " << e.what() << std::endl;
            return false;
        }
    }
}