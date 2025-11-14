//Linux
// g++ -I include/third_party -I include server_test.cpp -o server -lssl -lcrypto -lpthread

// Compilación Karol:
// g++ -D_WIN32_WINNT=0x0A00 -I include/third_party -I include server_test.cpp -o server -lssl -lcrypto -lpthread -lws2_32 -lcrypt32

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include <string>
#include <httplib.h>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    // Configurar servidor HTTPS en puerto 8443
    httplib::SSLServer svr("./cert.pem", "./key.pem");

    if (!svr.is_valid()) {
        std::cerr << "Error: No se pudo inicializar el servidor HTTPS" << std::endl;
        std::cerr << "Asegúrate de tener cert.pem y key.pem en el directorio actual" << std::endl;
        return 1;
    }

    std::cout << "Servidor HTTPS iniciado en https://localhost:8443" << std::endl;
    std::cout << "Esperando peticiones..." << std::endl;

    // Endpoint POST /init
    svr.Post("/init", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva petición POST /init ===" << std::endl;
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
            error_response["message"] = "JSON inválido";
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

    // Endpoint GET /test (adicional para pruebas)
    svr.Get("/test", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "\n=== Nueva petición GET /test ===" << std::endl;
        
        json response;
        response["status"] = "ok";
        response["message"] = "Servidor funcionando correctamente";
        
        res.set_content(response.dump(), "application/json");
        res.status = 200;
    });

    // Endpoint raíz
    svr.Get("/", [](const httplib::Request &req, httplib::Response &res) {
        res.set_content("Servidor HTTPS funcionando\n", "text/plain");
    });

    // Iniciar el servidor
    std::cout << "\nServidor escuchando en el puerto 8443..." << std::endl;
    svr.listen("0.0.0.0", 8443);

    return 0;
}