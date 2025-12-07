#include "client/commands.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem> 
#include <map> 

#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/scanner_codec.hpp" 
#include "client/comparator_codec.hpp"
#include "client/packer_codec.hpp"
#include "client/hasher_codec.hpp"
#include "client/key_loader.hpp"
#include "client/sign_codec.hpp" 

#include "client/verify_codec.hpp" 

namespace client::cmd {

    bool run_push(const std::string& project_name, const std::string& email, const std::string& directory, const std::string& key_path, const std::string& password) {
        std::cout << "\n--- Iniciando Push: " << project_name << " ---" << std::endl;
        std::cout << "Nombre: " << project_name << std::endl;
        std::cout << "Owner:  " << email << std::endl;
        
        std::filesystem::path base_path(directory);
        std::filesystem::path repo_path = base_path / project_name;
        
        // Validaciones Básicas
        if (!std::filesystem::exists(repo_path)) { 
            std::cerr << "[-] Error: No existe la carpeta del proyecto: " << repo_path << std::endl;
            return false; 
        }

        std::filesystem::path privateKeyPath = std::filesystem::path(key_path) / "private_ecdsa.key";
        if (!std::filesystem::exists(privateKeyPath)) {
            std::cerr << "[-] Error: No se encuentra 'private_ecdsa.key' en: " << key_path << std::endl;
            return false;
        }

        // Cargar Llave Privada
        client::key_loader::ECDSAPrivateKey privateKey;
        if (!client::key_loader::load_private_key(privateKeyPath.string(), privateKey)) {
            std::cerr << "[-] Error: La llave privada es invalida o corrupta." << std::endl;
            return false;
        }

        try {
            
            std::string hashedPass = client::hasher_codec::hash_sha256(password);
            
            auto payload_check = client::json_nlohmann::make_push_check_payload(project_name, email, hashedPass);
            auto response_check = client::http::post_json_https("/push/check", payload_check);
            
            if (!response_check.contains("status") || response_check["status"] != "success") {
                std::cerr << "[-] El servidor rechazo la solicitud." << std::endl;
                if(response_check.contains("message")) std::cerr << "    Motivo: " << response_check["message"] << std::endl;
                return false;
            }

            std::map<std::string, std::string> remote_files;
            if(response_check.contains("server_hashes")) {
                remote_files = response_check["server_hashes"].get<std::map<std::string, std::string>>();
            }

            
            std::cout << "[*] Escaneando cambios locales..." << std::endl;
            auto local_files = client::scanner::generate_file_map(repo_path.string());
            auto files_to_upload = client::comparator::compute_diff(local_files, remote_files);

            if (files_to_upload.empty()) {
                std::cout << "\n[+] El repositorio esta actualizado. No hay nada que subir." << std::endl;
                return true;
            }

            std::cout << " -> Se actualizaran " << files_to_upload.size() << " archivos." << std::endl;

            // EMpaquetar y firmar 
            std::string temp_tar = "push_upload.tar.gz";
            std::cout << "[*] Procesando archivos..." << std::endl;
            
            // Empaquetar
            auto original_path = std::filesystem::current_path();
            std::filesystem::current_path(repo_path);
            
            if (!client::packer::pack_files(files_to_upload, temp_tar)) {
                std::filesystem::current_path(original_path);
                return false;
            }
            
            // Crear tar
            std::string tar_abs_path = (original_path / temp_tar).string();
            std::filesystem::rename(temp_tar, tar_abs_path);
            std::filesystem::current_path(original_path); 

            // *********** VARIABLES DE VERIFICACION *****************
            // // En lugar de usar una fija, derivamos la pública de la privada cargada
            // client::verify_codec::ECDSAPublicKey publicKey;

            // // Crypto++ permite extraer la pública desde la privada así:
            // privateKey.MakePublicKey(publicKey);


            // Llave pública fija en Base64
            const std::string PUBLIC_KEY_BASE64 =
                "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE5GWzWLoiP7g60HPJUBd47Iqx7VZ5QcSWmPJ9jbWaju5pugOK4MpcfUIWhNi/N27tAGs+mV0UlW3+WjsDPP75Gg==";

            // Objeto llave publica
            client::verify_codec::ECDSAPublicKey publicKey;

            // Cargar la llave pública desde Base64 usando tu función
            if (!client::verify_codec::load_public_key_from_base64(PUBLIC_KEY_BASE64, publicKey)) {
                std::cerr << "[-] Error: No se pudo cargar o validar la llave pública fija." << std::endl;
                return false;
            }

            CryptoPP::AutoSeededRandomPool prng_temp;
            if (!publicKey.Validate(prng_temp, 3)) {
                std::cerr << "[-] Error: La llave pública es inválida." << std::endl;
                return false;
            }

            //---------------------------------------------------------------------



            // Firmar
            std::map<std::string, std::string> signatures_map;
            for (const auto& rel_path : files_to_upload) {
                std::string abs_file_path = (repo_path / rel_path).string();

                std::string signature;
                if (!client::sign_codec::sign_file(privateKey, abs_file_path, signature)) {
                    std::cerr << "[-] Error al firmar archivo: " << rel_path << std::endl;
                    return false;
                }

                // ---- VERIFICAR ----
                bool ok = client::verify_codec::verify_file_signature(publicKey, abs_file_path, signature);

                if (!ok) {
                    std::cerr << "[-] Error: La firma generada NO coincide con la llave pública." << std::endl;
                    std::cerr << "     Archivo: " << rel_path << std::endl;
                    return false;
                }

                //-------------------------------

                signatures_map[rel_path] = signature;
            }


            // Subir datos al servidor
            std::cout << "[*] Subiendo datos al servidor..." << std::endl;
            
            auto upload_payload = client::json_nlohmann::make_push_upload_payload(project_name, email, hashedPass, signatures_map);
            auto res_upload = client::http::upload_push_data("/push/upload", upload_payload, tar_abs_path);
            
            // Limpieza del archivo temporal
            if (std::filesystem::exists(tar_abs_path)) std::filesystem::remove(tar_abs_path);

            if (res_upload.contains("status") && res_upload["status"] == "success") {
                std::cout << "\n[EXITO] Push completado correctamente." << std::endl;
                return true;
            } else {
                std::cerr << "\n[-] Error en el servidor." << std::endl;
                if (res_upload.contains("message")) std::cerr << "Mensaje: " << res_upload["message"] << std::endl;
                return false;
            }

        } catch (const std::exception &e) {
            std::cerr << "[!] Error critico: " << e.what() << std::endl;
            return false;
        }
    }
}