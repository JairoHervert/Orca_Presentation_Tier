#include <iostream>
#include <filesystem> 
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
//#include "client/crypto_codec.hpp"
#include "client/generate_keypair_codec.hpp"
#include "client/files_codec.hpp"
#include "client/hasher_codec.hpp"

namespace client::cmd {

   bool run_keygen_ecdsa(const std::string& pathToOutput, const std::string& email, const std::string& password) {
        std::cout << std::endl << "\n --- Generacion de Llaves  ---" << std::endl;
        std::cout << "Usuario: " << email << std::endl;
        std::cout << "Claves: (ECDSA P-256) \n";
       
       std::filesystem::path outputDir(pathToOutput);
       if (!std::filesystem::exists(outputDir)) {
           std::cerr << "[-] Error Directorio no existe: " << pathToOutput << std::endl;
           return false; 
       }

       ECDSAKeyPair ECDSAkeyPair;
       try {
           ECDSAkeyPair = client::generate_keypair_codec::generate_ecdsa_keypair();
       } catch (...) { return false; }

       // Guardar
       std::filesystem::path privPath = outputDir / "private_ecdsa.key";
       
       if (!client::files_codec::save_string_to_file(ECDSAkeyPair.ECDSAprivateKey, privPath.string())) return false;
       

       std::string hashedPass = client::hasher::hash_sha256(password);
       std::cout << std::endl;
       
       try {
           auto payload = client::json_nlohmann::make_keygen_ecdsa_payload(ECDSAkeyPair.ECDSApublicKey, email, hashedPass);
           auto response = client::http::post_json_https("/user/add_kpub_ecdsa", payload);
           client::response_handler::handle_keygen_response(response);
       } catch (const std::exception& e) {
           std::cerr << "[!] Error de red: " << e.what() << std::endl;
       }
       return true;
   }

    bool run_keygen_rsa(const std::string& pathToOutput, const std::string& email, const std::string& password) {
        std::cout << "\n --- Generacion de Llaves  ---" << std::endl << std::endl;
        std::cout << "Usuario: " << email << std::endl;
        std::cout << "Claves: (RSA-OAEP 2048) \n"<< std::endl;

        // Validar Directorio
        std::filesystem::path outputDir(pathToOutput);
        if (!std::filesystem::exists(outputDir) || !std::filesystem::is_directory(outputDir)) {
            std::cerr << "[-] Error El directorio de salida no es valido: " << pathToOutput << std::endl;
            return false;
        }

        // Hashear password
        std::string hashedPass = client::hasher::hash_sha256(password);
        if (hashedPass.empty()) {
            std::cerr << "[-] Error Fallo interno al procesar password." << std::endl;
            return false;
        }

        // Generar Llaves
        RSAKeyPair RSAkeyPair;
        try {
            RSAkeyPair = client::generate_keypair_codec::generate_rsa_keypair();
            
            if (RSAkeyPair.RSApublicKey.empty()) throw std::runtime_error("Clave publica vacia");
        } catch (const std::exception& e) { 
            std::cerr << "[-] Error " << e.what() << std::endl;
            return false; 
        }

        
        try {
            auto payload = client::json_nlohmann::make_keygen_rsa_payload(RSAkeyPair.RSApublicKey, email, hashedPass);
            
            auto response = client::http::post_json_https("/user/add_kpub_rsa", payload);
            
            client::response_handler::handle_keygen_rsa_response(response);

            // Verificar Éxito del Servidor
            bool serverAccepted = false;
            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                serverAccepted = true;
            }

            //  GuardarSOLO si el servidor aceptó
            if (serverAccepted) {
                std::cout << "\n[+] Servidor acepto la clave RSA. Guardando en disco..." << std::endl;
                
                std::filesystem::path privPath = outputDir / "private_rsa.key";

                bool savePriv = client::files_codec::save_string_to_file(RSAkeyPair.RSAprivateKey, privPath.string());

                if (savePriv) {
                    std::cout << "  -> Clave Privada: " << privPath.string() << std::endl;
                    return true;
                } else {
                    std::cerr << "[-] Error de permisos al escribir archivos." << std::endl;
                    return false;
                }
            } else {
                return false;
            }

        } catch (const std::exception& e) {
            std::cerr << "[!] Error de Conexion " << e.what() << std::endl;
            return false;
        }
    }

   // --- Despachador Principal ---
   bool run_keygen(const std::string& keyType, const std::string& pathToOutput, const std::string& email, const std::string& password) {
       if (keyType == "ecdsa") {
           return run_keygen_ecdsa(pathToOutput, email, password);
       } else if (keyType == "rsa") {
           return run_keygen_rsa(pathToOutput, email, password);
       } else {
           std::cerr << "[!] Clave no resgistrada en el Sistema: " << keyType << " (Use 'ecdsa' o 'rsa')" << std::endl;
           return false;
       }
   }
}