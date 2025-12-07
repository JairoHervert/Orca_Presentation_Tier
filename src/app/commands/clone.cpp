#include <iostream>
#include <filesystem>
#include <string>

#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/unpacker_codec.hpp" 
#include "client/hasher_codec.hpp" 

namespace client::cmd {

    bool run_clone(const std::string& repo_name, const std::string& destination, const std::string& email, const std::string& password) {
        std::cout << "\n--- Clonando Repositorio ---" << std::endl;
        std::cout << "Proyecto: " << repo_name << std::endl;
        std::cout << "Destino:  " << destination << std::endl;

        try {
            // Hashear password
            std::string hashedPassword = client::hasher_codec::hash_sha256(password);
            if (hashedPassword.empty()) {
                std::cerr << "[-] Error interno de seguridad." << std::endl;
                return false;
            }

            // Payload
            auto payload = client::json_nlohmann::make_clone_payload(repo_name, email, hashedPassword);

            // Descargar
            nlohmann::json response = client::http::post_download_file("/repo/clone", payload, repo_name + ".tar");

            // Mostrar respuesta del servidor (Solo dice "Paquete recibido")
            client::response_handler::handle_clone_response(response);

            if (response.value("status", "error") != "ok") {
                return false; 
            }

            if (!response.contains("downloaded_file")) {
                std::cerr << "[-] Error interno: No se encuentra el archivo descargado." << std::endl;
                return false;
            }

            std::string downloaded_file = response["downloaded_file"];
            std::string tar_abs_path = std::filesystem::absolute(downloaded_file).string();

            std::cout << "\n[*] Procesando archivos (Desempaquetando)..." << std::endl;

            // Desempaquetar
            if (client::unpacker::unpack_file(tar_abs_path, destination)) {
                
                // Limpiar
                try {
                    if (std::filesystem::exists(tar_abs_path)) std::filesystem::remove(tar_abs_path);
                } catch (...) {}

                std::cout << "--------------------------------------" << std::endl;
                std::cout << "[+] Repositorio clonado exitosamente." << std::endl;
                std::cout << "     Tus archivos estan listos en: " << destination << std::endl;
                return true;

            } else {
                
                std::cerr << "--------------------------------------" << std::endl;
                std::cerr << "[-] Error critico: Se descargo el repositorio pero fallo la extraccion." << std::endl;
                std::cerr << "    El archivo dañado esta en: " << tar_abs_path << std::endl;
                return false;
            }

        } catch (const std::exception &e) {
            std::cerr << "[-] Excepcion: " << e.what() << std::endl;
            return false;
        }
    }
}