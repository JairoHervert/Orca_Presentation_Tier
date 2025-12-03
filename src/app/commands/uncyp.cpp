#include <iostream>
#include <filesystem>
#include <string>

#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp" 
#include "client/unpacker_codec.hpp" 
#include "client/hasher_codec.hpp" 
#include "client/decipher_AES_codec.hpp" 
#include "client/key_loader.hpp"         


namespace client::cmd {

    bool run_uncyp(const std::string& repo_name, const std::string& destination, 
                   const std::string& email, const std::string& password, 
                   const std::string& key_path) {

        std::cout << "\n--- Descifrando Repositorio ---" << std::endl;
        std::cout << "Proyecto: " << repo_name << std::endl;
        std::cout << "Destino:  " << destination << std::endl;

        try {

            // Ruta de llave AES
            std::filesystem::path dirPath(key_path);
            std::filesystem::path fullKeyPath = dirPath / "AES.key";

            // Verificar que el archivo exista 
            if (!std::filesystem::exists(fullKeyPath)) {
                std::cerr << "[-] Error: No se encuentra el archivo 'AES.key' en el directorio: " << key_path << std::endl;
                return false;
            }

            // Cargar la llave
            std::string aes_key_raw;
            // Usamos la funcion que lee y decodifica el Base64 a bytes crudos
            if (!client::key_loader::load_aes_key(fullKeyPath.string(), aes_key_raw)) {
                return false; // El loader ya imprimio el error especifico
            }

            std::string hashedPassword = client::hasher::hash_sha256(password);
            if (hashedPassword.empty()) {
                std::cerr << "[-] Error interno de seguridad (Hashing)." << std::endl;
                return false;
            }

            auto payload = client::json_nlohmann::make_unprotect_payload(repo_name, email, hashedPassword);

            // Descargar el archivo cifrado (.tar.enc)
            nlohmann::json response = client::http::post_download_file("/repo/unprotect", payload, repo_name + ".tar.enc");

            client::response_handler::handle_uncyp_response(response);

            // Verificar si hubo un error 
            if (response.value("status", "error") != "ok") {
                return false;
            }

            if (!response.contains("downloaded_file")) {
                std::cerr << "[-] Error: No se encuentra la ruta del archivo descargado." << std::endl;
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
            std::cout << "\n[*] Descifrando datos (Usando llave local)..." << std::endl;

            // Pasamos 'aes_key_raw' que contiene los bytes listos para usar
            if (!client::decipher_aes::decipher_AES_GCM(encrypted_abs_path, tar_file_path, aes_key_raw)) {
                std::cerr << "[-] Fallo el descifrado." << std::endl; 
                std::cerr << "    Causas probables: Llave incorrecta o archivo corrupto." << std::endl;
                
                // Limpieza parcial
                try { std::filesystem::remove(encrypted_abs_path); } catch(...) {}
                return false;
            }

            // Desempaquetar
            std::cout << " [*] Extrayendo archivos..." << std::endl;
            
            std::filesystem::path baseDest(destination);
            std::filesystem::path finalDest = baseDest / repo_name;

            // Pasamos la ruta nueva al unpacker
            if (client::unpacker::unpack_file(tar_file_path, finalDest.string())) {
                
                // Limpieza
                try {
                    if (std::filesystem::exists(encrypted_abs_path)) std::filesystem::remove(encrypted_abs_path);
                    if (std::filesystem::exists(tar_file_path)) std::filesystem::remove(tar_file_path);
                } catch (...) {}

                std::cout << "--------------------------------------" << std::endl;
                std::cout << "[OK] Repositorio recuperado exitosamente." << std::endl;
                std::cout << "     Ubicacion final: " << finalDest.string() << std::endl; 
                return true;

            } else {
                std::cerr << "[-] Error critico al desempaquetar el archivo." << std::endl;
                try { 
                    if (std::filesystem::exists(encrypted_abs_path)) std::filesystem::remove(encrypted_abs_path);
                    if (std::filesystem::exists(tar_file_path)) std::filesystem::remove(tar_file_path);
                } catch(...) {}
                return false;
            }

        } catch (const std::exception &e) {
            std::cerr << "[-] Excepcion critica en run_uncyp: " << e.what() << std::endl;
            return false;
        }
    }
}