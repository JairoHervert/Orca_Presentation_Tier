#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>
#include "client/client_https.hpp"
#include "../../include/third_party/dotenv.h"

namespace client::http {

   // Enviar peticiones post HTTPS
    nlohmann::json post_json_https(const std::string &path, const nlohmann::json &payload) {
        try {
            auto cli = conect(); // Ahora es un unique_ptr
            std::string payload_str = payload.dump();
            
            // Usar el puntero con ->
            auto res = cli->Post(path.c_str(), payload_str, "application/json");
            
            if (!res) {
                throw std::runtime_error("[!] Could not contact the server (Connection failed)");
            }

            if (res->status == 200 || res->status == 201 || res->status == 400 || res->status == 500 ) {
                try {
                    return nlohmann::json::parse(res->body);
                } catch (...) {
                    return nlohmann::json{
                        {"status", "error"},
                        {"message", res->body}
                    };
                }
            }
            
            return nlohmann::json::parse(res->body);
        } catch (const std::exception &e) {
            std::cerr << "[-] Error in post_json_https: " << e.what() << std::endl;
            throw;
        }
    }

        // Para clone y uncyp
    std::string extract_filename(const std::string& cd) {
        std::smatch m;
        std::regex re("filename=\"?([^\"]+)\"?");
        return (std::regex_search(cd, m, re) && m.size() > 1) ? m.str(1) : "";
    }

    std::string ensure_tar_extension(std::string name) {
        if (!std::filesystem::path(name).has_extension())
            name += ".tar";
        return name;
    }
        

    nlohmann::json post_download_file(const std::string& path, const nlohmann::json& payload, const std::string& default_path) {
        try {
            auto cli = conect();
            cli->set_read_timeout(120, 0); // Long timeout for downloads

            auto res = cli->Post(path.c_str(), payload.dump(), "application/json");

            // Connection Error
            if (!res) {
                return {
                    {"status", "error"},
                    {"message", "[!] Could not contact the server (Connection failed)"}
                };
            }

            if (res->status != 200) {
                try {
                    return nlohmann::json::parse(res->body);
                } catch (...) {
                    return {
                        {"status", "error"},
                        {"message", res->body}
                    };
                }
            }
            
            // Get filename from header
            std::string final_name = default_path;
            if (res->has_header("Content-Disposition")) {
                auto fname = extract_filename(res->get_header_value("Content-Disposition"));
                if (!fname.empty()) {
                    fname = std::filesystem::path(fname).filename().string(); 
                    final_name = ensure_tar_extension(fname);
                }
            }

            std::ofstream file(final_name, std::ios::binary);
            if (!file) {
                return {
                    {"status", "error"},
                    {"message", "[-] Local error: Could not create file " + final_name}
                };
            }

            file.write(res->body.data(), res->body.size());

            return {
                {"status", "ok"},
                {"message", "Repository downloaded successfully."},
                {"downloaded_file", final_name} 
            };

        } catch (const std::exception& e) {
            return {
                {"status", "error"},
                {"message", std::string("[-] Internal exception: ") + e.what()}
            };
        }
    }

    // ********* SE PUEDEN QUITAR **************
    
    // primera prueba para recibir una respuesta como string del server real
   std::string post_string_https(const std::string &path) {
      try {
         auto cli = conect(); // Ahora es un unique_ptr
         
         // Usar el puntero con ->
         auto res = cli->Post(path.c_str(), "", "application/json");
         
         if (!res || res->status != 200) {
            throw std::runtime_error("[-] Error en la respuesta del servidor");
         }
         
         return res->body;
      } catch (const std::exception &e) {
         std::cerr << "[-] Error en post_string_https: " << e.what() << std::endl;
         throw;
      }
   }

    // Retorna un puntero unico al cliente HTTPS conectado
    std::unique_ptr<httplib::SSLClient> conect() {
      try {
         // Cargar variables de entorno
         dotenv::init();
         std::string host = dotenv::getenv("HOST", "localhost");
         int port = std::stoi(dotenv::getenv("PORT", "443"));
         
         // Crear cliente HTTPS con puntero unico
         auto client_conection = std::make_unique<httplib::SSLClient>(host.c_str(), port);
         client_conection->set_read_timeout(5, 0);
         client_conection->enable_server_certificate_verification(false);
         
         return client_conection;
      } catch (const std::exception &e) {
         std::cerr << "Error al conectar: " << e.what() << std::endl;
         throw;
      }
   }

   nlohmann::json upload_push_data(const std::string &path, const nlohmann::json &metadata_payload, const std::string &tar_filepath) {
        try {
            auto cli = conect();
            std::ifstream file(tar_filepath, std::ios::binary | std::ios::ate);
            if (!file) throw std::runtime_error("No se pudo leer el tar");
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<char> buffer(size);
            if (!file.read(buffer.data(), size)) throw std::runtime_error("Error leyendo tar");
            std::string tar_content(buffer.begin(), buffer.end());

            std::vector<httplib::MultipartFormData> items;
            items.push_back({"metadata", metadata_payload.dump(), "", "application/json"});
            items.push_back({"archive", tar_content, "upload.tar.gz", "application/x-gzip"});

            auto res = cli->Post(path.c_str(), items);
            if (!res || res->status != 200) throw std::runtime_error("Error en upload");
            return nlohmann::json::parse(res->body);
        } catch (const std::exception &e) {
            std::cerr << "Error upload: " << e.what() << std::endl;
            throw;
        }
    }

}

   