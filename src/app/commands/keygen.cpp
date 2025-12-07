#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/files_codec.hpp"
#include "client/hasher_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/generate_keypair_codec.hpp"

namespace client::cmd {

   bool run_keygen_ecdsa(const std::string& pathToOutput, const std::string& email, const std::string& password) {
        std::cout << std::endl << "\n --- Key Generation ---" << std::endl;
        std::cout << "User: " << email << std::endl;
        std::cout << "Keys: (ECDSA P-256) \n";
       
        // Validar Directorio
        std::filesystem::path outputDir(pathToOutput);
        if (!std::filesystem::exists(outputDir)) {
            std::cerr << "\n[-] Error: Directory does not exist: " << pathToOutput << std::endl;
            return false; 
        }

        // Generar Llaves
        ECDSAKeyPair ECDSAkeyPair;
        try {
            ECDSAkeyPair = client::generate_keypair_codec::generate_ecdsa_keypair();
        } catch (...) { return false; }

        // Hashear password
        std::string hashedPass = client::hasher_codec::hash_sha256(password);
        std::cout << std::endl;
       
        try {
            // Enviar al Servidor
            auto payload = client::json_nlohmann::make_keygen_ecdsa_payload(ECDSAkeyPair.ECDSApublicKey, email, hashedPass);
            auto response = client::http::post_json_https("/user/add_kpub_ecdsa", payload);
            
            // Imprimir respuesta cruda o procesada
            client::response_handler::handle_keygen_response(response);

            // Verificar Éxito del Servidor
            bool serverAccepted = false;
            if (response.contains("status") && (response["status"] == "ok" || response["status"] == "success")) {
                serverAccepted = true;
            }

            // Guardar SOLO si el servidor aceptó
            if (serverAccepted) {
                std::cout << "\n[*] Server accepted ECDSA key. Saving to disk..." << std::endl;

                std::filesystem::path privPath = outputDir / "private_ecdsa.key";
                
                // Intentar guardar el archivo
                if (client::files_codec::save_string_to_file(ECDSAkeyPair.ECDSAprivateKey, privPath.string())) {
                     std::cout << "  -> Private Key: " << privPath.string() << std::endl;
                     return true;
                } else {
                     std::cerr << "[-] Permission error writing files." << std::endl;
                     return false;
                }
            } else {
                return false; 
            }

        } catch (const std::exception& e) {
            std::cerr << "[!] Network Error: " << e.what() << std::endl;
            return false;
        }
   }

    bool run_keygen_rsa(const std::string& pathToOutput, const std::string& email, const std::string& password) {
        std::cout << "\n --- Key Generation ---" << std::endl;
        std::cout << "User: " << email << std::endl;
        std::cout << "Keys: (RSA-OAEP 2048) \n"<< std::endl;

        // Validar Directorio
        std::filesystem::path outputDir(pathToOutput);
        if (!std::filesystem::exists(outputDir) || !std::filesystem::is_directory(outputDir)) {
            std::cerr << "\n[-] Error: Directory does not exist: " << pathToOutput << std::endl;
            return false;
        }

        // Hashear password
        std::string hashedPass = client::hasher_codec::hash_sha256(password);
        if (hashedPass.empty()) {
            std::cerr << "[-] Error: Internal failure processing password." << std::endl;
            return false;
        }

        // Generar Llaves
        RSAKeyPair RSAkeyPair;
        try {
            RSAkeyPair = client::generate_keypair_codec::generate_rsa_keypair();
            
            if (RSAkeyPair.RSApublicKey.empty()) throw std::runtime_error("Empty public key");
        } catch (const std::exception& e) { 
            std::cerr << "[-] Error: " << e.what() << std::endl;
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

            //  Guardar SOLO si el servidor aceptó
            if (serverAccepted) {
                std::cout << "\n[*] Server accepted RSA key. Saving to disk..." << std::endl;
                
                std::filesystem::path privPath = outputDir / "private_rsa.key";

                bool savePriv = client::files_codec::save_string_to_file(RSAkeyPair.RSAprivateKey, privPath.string());

                if (savePriv) {
                    std::cout << "  -> Private Key: " << privPath.string() << std::endl;
                    return true;
                } else {
                    std::cerr << "[-] Permission error writing files." << std::endl;
                    return false;
                }
            } else {
                return false;
            }

        } catch (const std::exception& e) {
            std::cerr << "[!] Connection Error: " << e.what() << std::endl;
            return false;
        }
    }

   bool run_keygen(const std::string& keyType, const std::string& pathToOutput, const std::string& email, const std::string& password) {
       if (keyType == "ecdsa") {
           return run_keygen_ecdsa(pathToOutput, email, password);
       } else if (keyType == "rsa") {
           return run_keygen_rsa(pathToOutput, email, password);
       } else {
           std::cerr << "[!] Key type not registered in system: " << keyType << " (Use 'ecdsa' or 'rsa')" << std::endl;
           return false;
       }
   }
}