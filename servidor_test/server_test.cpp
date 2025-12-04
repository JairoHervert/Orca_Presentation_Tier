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

    // ... (Tus includes y setup inicial igual) ...

    // --- Endpoint: /push/check ---
    svr.Post("/push/check", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n\n=== [POST] /push/check (Simulacion) ===" << std::endl;
        std::cout << "--- Headers ---" << std::endl;
        std::cout << "Content-Type: " << req.get_header_value("Content-Type") << std::endl;
        
        std::cout << "--- Body (JSON) ---" << std::endl;
        std::cout << req.body << std::endl; // Imprime el JSON crudo que manda el cliente

        json response;
        try {
            json payload = json::parse(req.body);
            
            // Validamos campos (Estilo CamelCase)
            if (!payload.contains("repoName") || !payload.contains("userEmail")) {
                std::cout << "(!) Faltan campos en el JSON" << std::endl;
                res.status = 400;
                res.set_content("{\"status\":\"error\",\"message\":\"Missing fields\"}", "application/json");
                return;
            }

            std::string project = payload["repoName"];
            std::cout << "-> Cliente pregunta por repo: " << project << std::endl;

            // RESPUESTA SIMULADA
            // El servidor dice que ya tiene estos archivos con estos hashes
            json server_files;
            server_files["src/main.cpp"] = "hash_viejo_12345"; 
            server_files["README.md"]    = "hash_viejo_67890";

            response["status"] = "success";
            response["server_hashes"] = server_files;
            
            std::cout << "-> Respondiendo con lista de archivos simulada." << std::endl;
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (...) { 
            res.status = 500; 
            res.set_content("{\"status\":\"error\"}", "application/json");
        }
    });

    // --- Endpoint: /push/upload ---
    svr.Post("/push/upload", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n\n=== [POST] /push/upload (Simulacion) ===" << std::endl;
        
        // 1. Verificar Metadata (JSON)
        if (req.has_file("metadata")) {
            const auto& file = req.get_file_value("metadata");
            std::cout << "--- Part: metadata (JSON) ---" << std::endl;
            
            try {
                // Imprimimos bonito el JSON para ver las firmas
                json meta = json::parse(file.content);
                std::cout << meta.dump(4) << std::endl; 
                
                if (meta.contains("files_signatures")) {
                    std::cout << "\n[OK] Se recibieron " << meta["files_signatures"].size() << " firmas." << std::endl;
                }
            } catch (...) {
                std::cout << file.content << std::endl; // Si falla parse, imprime crudo
            }
        } else {
            std::cout << "[!] Falta la parte 'metadata'" << std::endl;
        }

        // 2. Verificar Archivo (Binario)
        if (req.has_file("archive")) {
            const auto& file = req.get_file_value("archive");
            std::cout << "\n--- Part: archive (Binario .tar.gz) ---" << std::endl;
            std::cout << "Nombre del archivo: " << file.filename << std::endl;
            std::cout << "Tamano recibido:    " << file.content.size() << " bytes" << std::endl;
            
            // --- ESTO ES LO NUEVO: GUARDAR EL BINARIO PARA PROBAR ---
            std::string debug_filename = "debug_received_" + file.filename;
            std::ofstream out(debug_filename, std::ios::binary);
            if (out) {
                out.write(file.content.data(), file.content.size());
                out.close();
                std::cout << "[PRUEBA] Archivo binario guardado en disco como: " << debug_filename << std::endl;
                std::cout << "         -> Intenta abrirlo para verificar que no este corrupto." << std::endl;
            } else {
                std::cerr << "[ERROR] No se pudo guardar el archivo de prueba." << std::endl;
            }
            // ---------------------------------------------------------
        } else {
            std::cout << "[!] Falta la parte 'archive'" << std::endl;
        }

        // Respuesta final
        json response;
        response["status"] = "success";
        response["message"] = "Push recibido, firmas validadas y repositorio actualizado (Simulado)";
        
        res.set_content(response.dump(), "application/json");
        res.status = 200;
    });

    // --- CONFIG ---
    svr.Post("/nuser", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /nuser ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            
            std::string name = payload["data"]["name"];
            std::string email = payload["data"]["email"];
            std::string password = payload["data"]["password"];

            // SIMULACIÓN DE GUARDADO EN BD
            std::cout << "Guardando usuario en BD:" << std::endl;
            std::cout << "  Nombre: " << name << std::endl;
            std::cout << "  Email:  " << email << std::endl;
            std::cout << "  password:  " << password << std::endl;
            
            response["status"] = "success";
            response["message"] = "Usuario registrado correctamente.";
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (const std::exception &e) {
            std::cerr << "Error en nuser: " << e.what() << std::endl;
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

            // Recuperar datos de la tabla 'Commits' 
            json history = json::array();

            // Registro Simulado 
            history.push_back({
                {"email", "test@gmail.com"},
                {"file", "src/app/commands/clone.cpp"},
                {"date", "2025-08-20 02:50:00"},
                {"status", "Pending"}
            });


            // Registro Simulado
            history.push_back({
                {"email", "Joseg@gmail.com"},
                {"file", "src/app/commands/init.cpp"},
                {"date", "2025--20 04:01:00"},
                {"status", "Error"}
            });


            // Registro Simulado
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

    // --- REVOKE ---
        svr.Post("/revoke", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /revoke ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            
            std::string project = payload["data"]["project_name"];
            std::string email = payload["data"]["email"]; // Ahora buscamos "email"
            std::string scope = payload["data"]["scope"];
            
            std::cout << "Revocando acceso..." << std::endl;
            std::cout << "  Proyecto: " << project << std::endl;
            std::cout << "  Email:    " << email << std::endl;

            if (scope == "single_file") {
                std::string file = payload["data"]["file"];
                std::cout << "  Archivo:  " << file << std::endl;
            } else {
                std::cout << "  Alcance:  Completo (Todo el proyecto)" << std::endl;
            }
            
            response["status"] = "success";
            response["message"] = "Permisos revocados correctamente (Simulado).";
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (const std::exception &e) {
            res.status = 500;
        }
    });

    svr.Post("/grant", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /grant ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            
            std::string project = payload["data"]["project_name"];
            std::string email = payload["data"]["email"];
            std::string scope = payload["data"]["scope"];
            
            std::cout << "Asignando permisos..." << std::endl;
            std::cout << "  Proyecto: " << project << std::endl;
            std::cout << "  Usuario:  " << email << std::endl;

            if (scope == "single_file") {
                std::string file = payload["data"]["file"];
                std::cout << "  Archivo:  " << file << std::endl;
            } else {
                std::cout << "  Alcance:  Completo (Todo el proyecto)" << std::endl;
            }
            
            response["status"] = "success";
            response["message"] = "Permisos concedidos correctamente (Simulado).";
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (const std::exception &e) {
            res.status = 500;
        }
    });

    svr.Post("/drop", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /drop ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            
            std::string email = payload["data"]["email"];
            
            std::cout << "Solicitud de baja para: " << email << std::endl;
            
            response["status"] = "success";
            response["message"] = "Usuario dado de baja (status=Inactive) correctamente.";
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (const std::exception &e) {
            res.status = 500;
        }
    });

    svr.Post("/active", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /active ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            
            std::string email = payload["data"]["email"];
            
            std::cout << "Solicitud de activacion para: " << email << std::endl;
            
            response["status"] = "success";
            response["message"] = "Usuario activado (status=Active) correctamente.";
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;

        } catch (const std::exception &e) {
            res.status = 500;
        }
    });

    // --- REMOVE ---
    svr.Post("/remove", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva peticion POST /remove ===" << std::endl;
        json response;
        try {
            json payload = json::parse(req.body);
            
            std::string repo_name = payload["data"]["project_name"];
            std::string normalized_name = normalize_name(repo_name);
            std::filesystem::path repo_path = "./Repos/" + normalized_name;
            
            std::cout << "Solicitud de borrado para: " << repo_name << std::endl;

            if (std::filesystem::exists(repo_path)) {
                std::uintmax_t n = std::filesystem::remove_all(repo_path);
                std::cout << "Borrado completado. Archivos eliminados: " << n << std::endl;

                response["status"] = "success";
                response["message"] = "Repositorio eliminado exitosamente del servidor.";
                res.status = 200;
            } else {
                std::cout << "Error: Repositorio no encontrado." << std::endl;
                response["status"] = "error";
                response["message"] = "El repositorio no existe.";
                res.status = 404;
            }
            
            res.set_content(response.dump(), "application/json");

        } catch (const std::exception &e) {
            std::cerr << "Error en remove: " << e.what() << std::endl;
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