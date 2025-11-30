#include "client/commands.hpp"
#include <iostream>
#include <filesystem>
#include <string>

#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"
#include "client/hasher_codec.hpp"
#include "client/files_codec.hpp"
#include "client/key_loader.hpp"
#include "client/decipher_RSA_codec.hpp"
// Pruebas
#include "client/decipher_AES_codec.hpp"
#include "client/unpacker_codec.hpp"

namespace client::cmd {

    bool run_cypher_repo(const std::string& leader_email, const std::string& leader_password, 
                         const std::string& senior_email, const std::string& repo_name, 
                         const std::string& repo_tag, const std::string& raw_pathkey) { // Renombramos a raw_pathkey

        std::cout << "\n --- Protegiendo Repositorio (Cifrado) ---" << std::endl;
        std::cout << "Proyecto: " << repo_name << " (" << repo_tag << ")" << std::endl;
        std::cout << "Lider:    " << leader_email << std::endl;

        try {
            // --- LOGICA DE RUTAS (NUEVO) ---
            std::filesystem::path keyPath(raw_pathkey);

            // 1. Si el usuario nos dio un directorio (ej. .../Keys), le pegamos el nombre por defecto
            if (std::filesystem::is_directory(keyPath)) {
                keyPath /= "private_rsa.key"; // El operador /= pone el \ o / automáticamente
                std::cout << "[i] Ruta de carpeta detectada. Buscando llave en: " << keyPath.string() << std::endl;
            }

            // 2. Verificamos que el archivo final exista
            if (!std::filesystem::exists(keyPath)) {
                std::cerr << "[-] Error: No se encuentra el archivo de llave privada en: " << keyPath.string() << std::endl;
                return false;
            }
            // --------------------------------

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

                    // Recuramos la clave RSA privada 
                    client::decipher_RSA_codec::RSAPrivateKey RSAprivatekey;
                    
                    // --- CARGA SEGURA (CORREGIDO) ---
                    // Usamos keyPath.string() que ya tiene la ruta completa corregida
                    if (!client::key_loader::load_private_key(keyPath.string(), RSAprivatekey)) {
                        std::cerr << "[-] Fallo critico: No se pudo cargar la llave privada." << std::endl;
                        return false; 
                    }

                    std::string outPlaintext;
                    // Intentamos descifrar
                    if(!client::decipher_RSA_codec::OAEP_decryptFile(RSAprivatekey, encryptedKey, outPlaintext)) {
                         std::cerr << "[-] Fallo el descifrado. Verifica que sea la llave correcta." << std::endl;
                         return false;
                    }

                    // Usamos files_codec para guardar
                    if (client::files_codec::save_string_to_file(outPlaintext, filename)) {
                        std::cout << "[+] Clave guardada exitosamente en: " << filename << std::endl;
                        std::cout << "Guardala en un lugar seguro. Solo la llave privada RSA del lider puede descifrarla." << std::endl;

                       // ---------------------------------------------------------
                        // --- ZONA DE PRUEBAS (TESTING) ---
                        // ---------------------------------------------------------
                        std::cout << "\n----------------------------------------------" << std::endl;
                        std::cout << "[TEST] Quieres probar el ciclo completo (Descifrar -> Desempaquetar)? [s/n]: ";
                        char testOpt;
                        std::cin >> testOpt;

                        if (testOpt == 's' || testOpt == 'S') {
                            
                            // RUTAS (Puedes hacerlas dinámicas luego, por ahora hardcoded para la prueba)
                            std::string inputEncFile = "C:\\Users\\kgonz\\Desktop\\Repos\\enc\\Example04_v1.f2.tar.enc";
                            std::string outputDecFile = "C:\\Users\\kgonz\\Desktop\\Repos\\enc\\Example04_v1.f2.tar"; // Archivo temporal
                            std::string outputDir   = "C:\\Users\\kgonz\\Desktop\\Repos\\enc\\Example04_Restored"; // Carpeta final

                            std::cout << "\n1. Descifrando AES-GCM..." << std::endl;
                            
                            // 1. DESCIFRAR
                            bool decryptSuccess = client::decipher_aes::decipher_AES_GCM(inputEncFile, outputDecFile, outPlaintext);
                            
                            if (decryptSuccess) {
                                std::cout << "[OK] Archivo descifrado: " << outputDecFile << std::endl;

                                // 2. DESEMPAQUETAR
                                std::cout << "2. Desempaquetando contenido..." << std::endl;
                                
                                if (client::unpacker::unpack_file(outputDecFile, outputDir)) {
                                    
                                    // 3. LIMPIEZA (BORRAR EL .TAR)
                                    std::cout << "3. Limpiando archivos temporales..." << std::endl;
                                    try {
                                        if (std::filesystem::remove(outputDecFile)) {
                                            std::cout << "[+] Archivo temporal (.tar) eliminado correctamente." << std::endl;
                                        }
                                    } catch (const std::filesystem::filesystem_error& e) {
                                        std::cerr << "[!] No se pudo borrar el archivo temporal: " << e.what() << std::endl;
                                    }

                                    std::cout << "\n[EXITO TOTAL] Tu repositorio ha sido recuperado en:\n    " << outputDir << std::endl;

                                } else {
                                    std::cerr << "[FAIL] Fallo al desempaquetar el archivo tar." << std::endl;
                                }

                            } else {
                                std::cerr << "[FAIL] No se pudo descifrar el archivo AES." << std::endl;
                            }
                        }
                        // ---------------------------------------------------------

                    } else {
                        std::cerr << "[-] Error No se pudo guardar el archivo." << std::endl;
                        return false;
                    }

                } else {
                    std::cout << "[!] Operacion finalizada sin guardar la clave localmente." << std::endl;
                    return false;
                }
            }

            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error en cypher_repo: " << e.what() << std::endl;
            return false;
        }
    }
}