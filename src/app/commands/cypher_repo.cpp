#include "client/commands.hpp"
#include <iostream>
#include <filesystem>
#include <string>

#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp"
#include "client/files_codec.hpp"

namespace client::cmd {

    bool run_cypher_repo(const std::string& leader_email, const std::string& leader_password, const std::string& senior_email, const std::string& repo_name, const std::string& repo_tag) {
        std::cout << "\n --- Protegiendo Repositorio (Cifrado) ---" << std::endl;
        std::cout << "Proyecto: " << repo_name << " (" << repo_tag << ")" << std::endl;
        std::cout << "Lider:    " << leader_email << std::endl;

        try {
            // Hashear password del líder
            std::string hashedPassword = client::hasher::hash_sha256(leader_password);
            if (hashedPassword.empty()) {
                std::cerr << "Error interno al procesar password." << std::endl;
                return false;
            }

            nlohmann::json payload = client::json_nlohmann::make_cypher_repo_payload(leader_email, hashedPassword, senior_email, repo_name, repo_tag);


            nlohmann::json response = client::http::post_json_https("/repo/protect", payload);

            // Mostrar respuesta visual
            client::response_handler::handle_cypher_repo_response(response);

            // Verificamos si el servidor mandó la clave cifrada
            if (response.contains("aes_rsa_key") && response.contains("status") && response["status"] == "ok") {
                
                std::string encryptedKey = response["aes_rsa_key"];

                std::cout << "Deseas guardar esta clave en un archivo ahora [s/n]: ";
                char opt;
                std::cin >> opt;

                if (opt == 's' || opt == 'S') {
                    std::string filename;
                    std::cout << "Ingresa el nombre del archivo (ej. repo_key): ";
                    std::cin >> filename;
                    filename += ".key";

                    

                    // Usamos files_codec para guardar
                    if (client::files_codec::save_string_to_file(encryptedKey, filename)) {
                        std::cout << "[+] Clave guardada exitosamente en: " << filename << std::endl;
                        std::cout << "Guardala en un lugar seguro. Solo la llave privada RSA del lider puede descifrarla." << std::endl;
                    } else {
                        std::cerr << "[-] Error No se pudo guardar el archivo." << std::endl;
                    }
                } else {
                    std::cout << "[!] Operacion finalizada sin guardar la clave localmente." << std::endl;
                }
            }

            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error en cypher_repo: " << e.what() << std::endl;
            return false;
        }
    }

}
