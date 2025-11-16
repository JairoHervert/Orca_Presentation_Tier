#include <iostream>
#include <filesystem>
#include "client/commands.hpp"
#include "client/json_codec.hpp"
#include "client/response_handler.hpp"
#include "client/downloader.hpp"     // Para download_file()
#include "client/client_https.hpp"   // Para conect()

namespace client::cmd {
    bool run_clone(const std::string& repo_name, const std::string& destination) {
        std::cout << "Clonando repositorio: " << repo_name << std::endl;
        std::cout << "Directorio de destino: " << destination << std::endl;

        try {
            nlohmann::json payload = client::json_nlohmann::make_clone_payload(repo_name, destination);
            std::cout << "Enviando peticion de clonado..." << std::endl;

            //Enviar la peticion POST /clone
            nlohmann::json response = client::http::post_json_https("/clone", payload);

            std::cout << "\nRespuesta del servidor " << std::endl;
            client::response_handler::handle_clone_response(response);

            // Verificar si la primera peticion fue exitosa
            if (response.contains("status") && response["status"] == "success") {

                std::cout << "\nIniciando descarga ..." << std::endl;

                // Extraer los datos para la descarga
                std::string archive_file = response["archive_file"];
                std::string download_path = "/download/" + archive_file;

                // Construir la ruta de guardado local
                std::filesystem::path save_path = std::filesystem::path(destination) / archive_file;

                // Pedir la conexión a la capa de transporte
                auto cli = client::http::conect();
                
                // Llamar a la herramienta 'core', pasándole la conexión
                int status_code = client::core::download_file(*cli, download_path, save_path.string());

                if (status_code == 200) {
                    std::cout << "\n¡Éxito! Repositorio clonado en: " << save_path.string() << std::endl;
                    // Aquí pondremos la lógica de descompresión (Paso 5)
                } else {
                    std::cerr << "\nError: La descarga del repositorio falló (Status: " << status_code << ")." << std::endl;
                    // Borramos el archivo incompleto si la descarga falló
                    std::filesystem::remove(save_path.string());
                }

            } else {
                std::cerr << "No se puede continuar debido a un error del servidor." << std::endl;
            }

            return true;

        } catch (const std::exception &e) {
            std::cerr << "Error al clonar el repositorio: " << e.what() << std::endl;
            return false;
        }
    }
}