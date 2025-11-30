#include "client/decipher_AES_codec.hpp"

namespace client::cmd {

    bool run_cypher_repo(){
        // ---------------------------------------------------------
                        // --- ZONA DE PRUEBAS (TESTING) ---
                        // ---------------------------------------------------------
                        std::cout << "\n----------------------------------------------" << std::endl;
                        std::cout << "[TEST] Quieres probar descifrar el archivo de prueba ahora? [s/n]: ";
                        char testOpt;
                        std::cin >> testOpt;

                        if (testOpt == 's' || testOpt == 'S') {
                            // Ruta HARDCODED para la prueba que pediste
                            std::string inputEncFile = "C:\\Users\\kgonz\\Desktop\\Repos\\enc\\Example04_v1.f.tar.enc";
                            // Le quitamos el .enc para el output
                            std::string outputDecFile = "C:\\Users\\kgonz\\Desktop\\Repos\\enc\\Example04_v1.f_decrypted.tar";

                            std::cout << "Descifrando: " << inputEncFile << "..." << std::endl;
                            
                            // Pasamos outPlaintext (que es la llave AES que acabamos de recuperar del RSA)
                            bool result = client::decipher_aes::decipher_AES_GCM(inputEncFile, outputDecFile, outPlaintext);
                            
                            if (result) {
                                std::cout << "[SUCCESS] Archivo descifrado correctamente en: " << outputDecFile << std::endl;
                            } else {
                                std::cerr << "[FAIL] No se pudo descifrar el archivo. La llave o el archivo estan corruptos." << std::endl;
                            }
                        }
                        // ---------------------------------------------------------
    }

}