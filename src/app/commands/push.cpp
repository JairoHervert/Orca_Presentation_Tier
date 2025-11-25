#include "client/commands.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem> 
#include <map> 
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher.hpp" 
#include "client/comparator.hpp"
#include "client/crypto_codec.hpp"
#include "client/packer.hpp"

namespace client::cmd {
    bool run_push(const std::string& project_name, const std::string& directory, const std::string& key_path, const std::string& password) {
        std::cout << "--- Iniciando Push ---" << std::endl;
        std::filesystem::path base_path(directory);
        std::filesystem::path repo_path = base_path / project_name;
        
        if (!std::filesystem::exists(repo_path)) { return false; }

        // Validar llave privada
        std::filesystem::path privateKeyFile = std::filesystem::path(key_path) / "private_ecdsa.key";
        if (!std::filesystem::exists(privateKeyFile)) {
            std::cerr << "[Error] No se encuentra 'private.key' en: " << key_path << std::endl;
            return false;
        }

        try {
            // FASE 1: Check
            std::cout << "[1] Consultando servidor..." << std::endl;
            auto payload_check = client::json_nlohmann::make_push_check_payload(project_name);
            auto response_check = client::http::post_json_https("/push/check", payload_check);
            
            client::response_handler::handle_push_check_response(response_check);
            if (!response_check.contains("status") || response_check["status"] != "success") return false;

            std::map<std::string, std::string> remote_files;
            if(response_check.contains("server_hashes")) {
                remote_files = response_check["server_hashes"].get<std::map<std::string, std::string>>();
            }

            // FASE 2: Calcular y Comparar
            std::cout << "[2] Calculando cambios locales..." << std::endl;
            auto local_files = client::codec::generate_file_map(repo_path.string());
            auto files_to_upload = client::codec::compute_diff(local_files, remote_files);

            if (files_to_upload.empty()) {
                std::cout << "\nTodo actualizado." << std::endl;
                return true;
            }

            // Cargar Llave
            std::cout << "[Seguridad] Cargando llave privada..." << std::endl;
            client::crypto_codec::PrivateKey privateKey;
            if (!client::crypto_codec::load_private_key(privateKeyFile.string(), privateKey)) {
                return false;
            }

            // Empaquetar y Firmar
            std::string temp_tar = "upload_temp.tar.gz";
            std::cout << "[3] Procesando " << files_to_upload.size() << " archivos..." << std::endl;
            
            auto original_path = std::filesystem::current_path();
            std::filesystem::current_path(repo_path);
            
            // Empaquetar
            if (!client::codec::pack_files(files_to_upload, temp_tar)) {
                std::filesystem::current_path(original_path);
                return false;
            }
            std::string tar_abs_path = std::filesystem::absolute(temp_tar).string();

            // Firmar
            std::map<std::string, std::string> metadata;
            for(const auto& relative_path : files_to_upload) {
                // Firmamos el ARCHIVO en disco
                std::string signature = client::crypto_codec::sign_file(privateKey, relative_path);
                metadata[relative_path] = signature;
            }
            std::filesystem::current_path(original_path);

            // FASE 4: Subir
            auto upload_payload = client::json_nlohmann::make_push_upload_payload(project_name, metadata, password);
            std::cout << "[4] Subiendo..." << std::endl;
            auto res_upload = client::http::upload_push_data("/push/upload", upload_payload, tar_abs_path);
            
            if (res_upload.contains("status") && res_upload["status"] == "success") {
                std::cout << "Exito: " << res_upload["message"] << std::endl;
            } else {
                std::cerr << "Error en subida." << std::endl;
            }
            
            std::filesystem::remove(tar_abs_path);
            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error critico: " << e.what() << std::endl;
            return false;
        }
    }
}