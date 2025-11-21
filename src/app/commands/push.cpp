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
#include "client/packer.hpp"

namespace client::cmd {
    bool run_push(const std::string& project_name, const std::string& directory) {
        std::cout << "--- Iniciando Push ---" << std::endl;
        std::filesystem::path base_path(directory);
        std::filesystem::path repo_path = base_path / project_name;
        
        if (!std::filesystem::exists(repo_path)) { 
            std::cerr << "Error: No se encuentra la carpeta " << repo_path.string() << std::endl;
            return false; 
        }

        try {
            // Consultar (CHECK)
            std::cout << "Consultando Hashes en el servidor..." << std::endl;
            auto payload = client::json_nlohmann::make_push_check_payload(project_name);
            auto response = client::http::post_json_https("/push/check", payload);
            
            client::response_handler::handle_push_check_response(response);
            if (!response.contains("status") || response["status"] != "success") return false;

            std::map<std::string, std::string> remote_files;
            if(response.contains("server_hashes")) {
                remote_files = response["server_hashes"].get<std::map<std::string, std::string>>();
            }

            // Calcular y Comparar los Hashes
            std::cout << " Calculando hashes locales..." << std::endl;
            auto local_files = client::codec::generate_file_map(repo_path.string());
            auto files_to_upload = client::codec::compute_diff(local_files, remote_files);

            if (files_to_upload.empty()) {
                std::cout << "\nTodo actualizado." << std::endl;
                return true;
            }

/// ----------------------------------ESTUDIAR------------------------------------------------------------
            // Empaquetar y Subir (UPLOAD)
            std::string temp_tar = "upload_temp.tar.gz";
            std::cout << " Empaquetando " << files_to_upload.size() << " archivos..." << std::endl;
            
            auto original_path = std::filesystem::current_path();
            std::filesystem::current_path(repo_path);
            if (!client::codec::pack_files(files_to_upload, temp_tar)) {
                std::filesystem::current_path(original_path);
                return false;
            }
            std::string tar_abs_path = std::filesystem::absolute(temp_tar).string();
            std::filesystem::current_path(original_path);

            std::map<std::string, std::string> metadata;
            for(const auto& f : files_to_upload) metadata[f] = local_files[f];
            auto upload_payload = client::json_nlohmann::make_push_upload_payload(project_name, metadata);

            std::cout << " Subiendo..." << std::endl;
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