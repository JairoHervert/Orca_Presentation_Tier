#include <iostream>
#include <filesystem>

#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/client_https.hpp"
#include "client/response_handler.hpp"

#include "client/crypto_codec.hpp"
#include "client/files_codec.hpp"

namespace client::cmd {
   bool run_keygen(const std::string& keyType, const std::string &pathToOutput, const std::string& email, const std::string& password) {

      if (keyType == "ecdsa") {
         std::cout << "Generando par de claves ECDSA..." << std::endl;

         // Validar que la ruta exista 
         std::filesystem::path outputDir(pathToOutput);
         
         // Verificar si existe la carpeta
         if (!std::filesystem::exists(outputDir)) {
             std::cerr << "Error: El directorio de salida NO existe: " << outputDir << std::endl;
             std::cerr << "Por favor, crea la carpeta antes de generar las llaves." << std::endl;
             return false;
         }

         // Verificar si es un directorio válido
         if (!std::filesystem::is_directory(outputDir)) {
             std::cerr << "Error: La ruta especificada no es una carpeta valida." << std::endl;
             return false;
         }
         
         // Generacion de claves ECDSA
         ECDSAKeyPair keyPair = client::crypto_codec::generate_ecdsa_keypair();

         // Construimos las rutas completas para las claves (.key)
         std::filesystem::path privPath = outputDir / "private_ecdsa.key";
         std::filesystem::path pubPath  = outputDir / "public_ecdsa.key";

          // Guardar Privada
         if (client::files_codec::save_string_to_file(keyPair.privateKey, privPath.string())) {
             std::cout << "Clave privada guardada en: " << privPath.string() << std::endl;
         } else {
             std::cerr << "Error: No se pudo guardar la clave privada." << std::endl;
             return false;
         }

         // Guardar Pública (Para validación)
         if (client::files_codec::save_string_to_file(keyPair.publicKey, pubPath.string())) {
             std::cout << "Clave publica guardada en: " << pubPath.string() << std::endl;
         } else {
             std::cerr << "Error: No se pudo guardar la clave publica." << std::endl;
         }

         
         // Llamar a la función específica para generar par de claves ECDSA
         nlohmann::json response = client::http::post_json_https("/user/add_kpub_ecdsa", client::json_nlohmann::make_keygen_ecdsa_payload(keyPair.publicKey, email, password));
         std::cout << "Respuesta del servidor: " << response.dump() << std::endl;

         client::response_handler::handle_keygen_response(response);
         std::cout << "Clave pública enviada al servidor. Sali de la funcion" << std::endl;


      } else if (keyType == "aes") {
         std::cout << "Generando clave AES..." << std::endl;
         // Llamar a la función específica para generar clave AES
         // Por ejemplo: generate_aes_key(outputDir);
         std::cout << "Clave AES guardada en: " << pathToOutput << std::endl;
      } else {
         std::cerr << "Tipo de clave no soportado: " << keyType << std::endl;
         return false;
      }


      return true;
   }
}