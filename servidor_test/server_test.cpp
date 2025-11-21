//Linux
// g++ -I include/third_party -I include server_test.cpp -o server -lssl -lcrypto -lpthread

// Compilacion Karol:
// cd C:/Users/kgonz/Desktop/OrcaProject/Orca_Presentation_Tier/servidor_test
// g++ -D_WIN32_WINNT=0x0A00 -I include/third_party -I include server_test.cpp -o server -lssl -lcrypto -lpthread -lws2_32 -lcrypt32

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include <string>
#include <httplib.h>
#include "json.hpp"

#include <fstream>

using json = nlohmann::json;

// Normaliza un nombre de repositorio a minusculas.
std::string normalize_name(const std::string& repo_name) {
    std::string normalized = repo_name;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return normalized;
}

int main() {
    // Configurar servidor HTTPS en puerto 8443
    httplib::SSLServer svr("./cert.pem", "./key.pem");

    if (!svr.is_valid()) {
        std::cerr << "Error: No se pudo inicializar el servidor HTTPS" << std::endl;
        std::cerr << "Asegurate de tener cert.pem y key.pem en el directorio actual" << std::endl;
        return 1;
    }

    std::cout << "Servidor HTTPS iniciado en https://localhost:8443" << std::endl;
    std::cout << "Esperando peticiones..." << std::endl;

    // Endpoint POST /init
    svr.Post("/init", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /init ===" << std::endl;
        std::cout << "Content-Type: " << req.get_header_value("Content-Type") << std::endl;
        std::cout << "Body recibido: " << req.body << std::endl;

        try {
            // Parsear el JSON recibido
            json payload = json::parse(req.body);
            std::cout << "JSON parseado exitosamente" << std::endl;
            std::cout << "Datos recibidos: " << payload.dump(2) << std::endl;

            // Crear respuesta JSON
            json response;
            response["status"] = "success";
            response["message"] = "Repositorio inicializado correctamente";
            response["received_data"] = payload;
            response["timestamp"] = std::time(nullptr);

            // Enviar respuesta
            res.set_content(response.dump(), "application/json");
            res.status = 200;
            
            std::cout << "Respuesta enviada: " << response.dump(2) << std::endl;

        } catch (const json::parse_error &e) {
            std::cerr << "Error al parsear JSON: " << e.what() << std::endl;
            
            json error_response;
            error_response["status"] = "error";
            error_response["message"] = "JSON invalido";
            error_response["error"] = e.what();
            
            res.set_content(error_response.dump(), "application/json");
            res.status = 400;
        } catch (const std::exception &e) {
            std::cerr << "Error del servidor: " << e.what() << std::endl;
            
            json error_response;
            error_response["status"] = "error";
            error_response["message"] = "Error interno del servidor";
            
            res.set_content(error_response.dump(), "application/json");
            res.status = 500;
        }
    });

    // Endpoint POST /clone
    svr.Post("/clone", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /clone ===" << std::endl;
        std::cout << "Body recibido: " << req.body << std::endl;

        json response; 
        try {
            json payload = json::parse(req.body);
            std::cout << "JSON parseado exitosamente" << std::endl;

            std::string repo_name = payload["data"]["repo"];
            std::string destination_path = payload["data"]["destination"];

            std::cout << "Peticion para clonar '" << repo_name << "' en '" << destination_path << "'" << std::endl;

            // normalizacion de repo_name
            std::string normalized_name = normalize_name(repo_name);
   
            
            std::string base_path = "./Repos/";
            std::filesystem::path repo_path = base_path + repo_name;

            if (std::filesystem::exists(repo_path) && std::filesystem::is_directory(repo_path)) {

                std::cout << "Repositorio '" << normalized_name << "' encontrado." << std::endl;

                // Comprimir el repositorio en un archivo .tar
                std::string timestamp = std::to_string(std::time(nullptr));
                std::string tar_filename = normalized_name + "_" + timestamp + ".tar.gz";
                std::string base_dir = "./Repos";

                std::string command_str = "tar -czf " + tar_filename + " -C " + base_dir + " " + normalized_name;

                std::cout << "Ejecutando comando: " << command_str << std::endl;
                int result = std::system(command_str.c_str());

                if (result == 0) {
                    std::cout << "Archivo " << tar_filename << " creado exitosamente." << std::endl;

                    response["status"] = "success";
                    response["message"] = "Repositorio comprimido. Listo para descargar.";
                    response["repo_found"] = true;
                    response["archive_file"] = tar_filename;
                    response["timestamp"] = std::time(nullptr);

                    res.set_content(response.dump(), "application/json");
                    res.status = 200; 

                } else {
                    std::cout << "Error al crear el archivo .tar" << std::endl;
                    response["status"] = "error";
                    response["message"] = "Error del servidor: No se pudo comprimir el repositorio.";
                    response["repo_found"] = true;

                    res.set_content(response.dump(), "application/json");
                    res.status = 500; 
                }

            } else {
                std::cout << "Error: Repositorio '" << normalized_name << "' NO encontrado en " << repo_path << std::endl;

                response["status"] = "error";
                response["message"] = "Repositorio no encontrado en el servidor.";
                response["repo_found"] = false;
                response["requested_repo"] = repo_name;

                res.set_content(response.dump(), "application/json");
                res.status = 404;
            }

            std::cout << "Respuesta enviada: " << response.dump(2) << std::endl;

        } catch (const json::parse_error &e) {
            std::cerr << "Error al parsear JSON: " << e.what() << std::endl;
            response["status"] = "error";
            response["message"] = "JSON invalido";
            res.set_content(response.dump(), "application/json");
            res.status = 400;
        } catch (const std::exception &e) {
            std::cerr << "Error del servidor: " << e.what() << std::endl;
            response["status"] = "error";
            response["message"] = "Error interno del servidor";
            res.set_content(response.dump(), "application/json");
            res.status = 500;
        }
    });

    // Endpoint GET /download/[filename]
    svr.Get("/download/(.+)", [](const httplib::Request &req, httplib::Response &res) {

        std::string filename = req.matches[1].str();
        std::cout << "\n=== Nueva peticion GET /download/" << filename << " ===" << std::endl;

        if (filename.find("..") != std::string::npos) {
            std::cerr << "¡ALERTA DE SEGURIDAD! Se denego la peticion de Path Traversal: " << filename << std::endl;
            res.set_content("Peticion invalida: Path Traversal denegado.", "text/plain");
            res.status = 400;
            return;
        }

        // Construir la ruta al archivo
        std::filesystem::path file_path = "./" + filename;

        // Verificar si el archivo existe
        if (std::filesystem::exists(file_path) && !std::filesystem::is_directory(file_path)) {

            // Leer el archivo del disco en modo binario
            std::ifstream file(file_path, std::ios::binary);
            if (file) {
                std::cout << "Archivo '" << filename << "' encontrado. Enviando..." << std::endl;

                // Copiar el contenido del archivo a un stream
                std::stringstream buffer;
                buffer << file.rdbuf();
                file.close();

                // Enviar el contenido del archivo como respuesta
                res.set_content(buffer.str(), "application/octet-stream");
                res.status = 200;

                try {
                    std::filesystem::remove(file_path);
                    std::cout << "Limpiando archivo temporal: " << filename << std::endl;
                } catch (const std::filesystem::filesystem_error& e) {
                    std::cerr << "Advertencia: No se pudo borrar el archivo temporal " << filename << ": " << e.what() << std::endl;
                }

            } else {
                // Error si no se puede leer el archivo (aunque exista)
                std::cerr << "Error: No se pudo leer el archivo '" << filename << "'" << std::endl;
                res.set_content("Error interno al leer el archivo", "text/plain");
                res.status = 500;
            }

        } else {
            // Error si el archivo .tar.gz no se encuentra
            std::cerr << "Error: Peticion de descarga para un archivo no existente: '" << filename << "'" << std::endl;
            res.set_content("Archivo no encontrado", "text/plain");
            res.status = 404;
        }
    });

    // Push check
    svr.Post("/push/check", [](const httplib::Request &req, httplib::Response &res) {
        json response;
        try {
            json payload = json::parse(req.body);
            std::string project = payload["data"]["project_name"];
            std::cout << "Solicitud hashes para: " << project << std::endl;

            json server_files;
            
            // Hash real que me diste:
            server_files["Orcaleros.cpp"] = "0f38d98e049d1719f38818e752f44dea0edde1d2d6506c3f98577e33847c6fd9";

            response["status"] = "success";
            response["server_hashes"] = server_files;
            
            std::cout << "Enviando hashes simulados (incluyendo Orcaleros.cpp igual)..." << std::endl;
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (...) { res.status = 500; }
    });

    // PUSH UPLOAD
    svr.Post("/push/upload", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "Recibiendo upload..." << std::endl;
        if (req.files.find("metadata") != req.files.end() && req.files.find("archive") != req.files.end()) {
            auto metadata = req.files.find("metadata")->second.content;
            auto archive_size = req.files.find("archive")->second.content.size();
            std::cout << "Metadata: " << metadata << std::endl;
            std::cout << "Archivo recibido: " << archive_size << " bytes." << std::endl;
            res.set_content("{\"status\":\"success\", \"message\":\"Recibido OK\"}", "application/json");
        } else {
            res.status = 400;
        }
    });

    // --- CONFIG ---
    svr.Post("/config", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /config ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            
            std::string name = payload["data"]["name"];
            std::string email = payload["data"]["email"];

            // SIMULACIÓN DE GUARDADO EN BD
            std::cout << "Guardando usuario en BD:" << std::endl;
            std::cout << "  Nombre: " << name << std::endl;
            std::cout << "  Email:  " << email << std::endl;

            // Aquí harías tu INSERT INTO Users...
            
            response["status"] = "success";
            response["message"] = "Usuario registrado correctamente.";
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (const std::exception &e) {
            std::cerr << "Error en config: " << e.what() << std::endl;
            res.status = 500;
        }
    });

    svr.Post("/log", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /log ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            std::string project = payload["data"]["project_name"];
            
            std::cout << "Consultando DB para proyecto: " << project << std::endl;

            // SIMULACIÓN: Recuperar datos de la tabla 'Commits' join 'Users' join 'SourceFiles'
            json history = json::array();

            // Registro Simulado 1
            history.push_back({
                {"email", "test@gmail.com"},
                {"file", "src/app/commands/clone.cpp"},
                {"date", "2025-08-20 02:50:00"},
                {"status", "Pending"}
            });


            // Registro Simulado 2
            history.push_back({
                {"email", "Joseg@gmail.com"},
                {"file", "src/app/commands/init.cpp"},
                {"date", "2025--20 04:01:00"},
                {"status", "Error"}
            });


            // Registro Simulado 3 (Del push que acabamos de hacer teóricamente)
            history.push_back({
                {"email", "karolruizg@gmail.com"},
                {"file", "src/app/commands/push.cpp"},
                {"date", "2025-02-20 11:30:00"},
                {"status", "Accepted"}
            });

            response["status"] = "success";
            response["history"] = history;
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (...) {
            res.status = 500;
        }
    });


    // Endpoint GET /test (adicional para pruebas)
    svr.Get("/test", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion GET /test ===" << std::endl;
        
        json response;
        response["status"] = "ok";
        response["message"] = "Servidor funcionando correctamente";
        
        res.set_content(response.dump(), "application/json");
        res.status = 200;
    });

    // Endpoint raiz
    svr.Get("/", [](const httplib::Request &req, httplib::Response &res) {
        res.set_content("Servidor HTTPS funcionando\n", "text/plain");
    });

    // Iniciar el servidor
    std::cout << "\nServidor escuchando en el puerto 8443..." << std::endl;
    svr.listen("0.0.0.0", 8443);

    return 0;
}