#include "client/colors.hpp" 
#include "client/client_https.hpp"
#include "../../include/third_party/dotenv.h"
namespace client::http {

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
            // ERROR CRÍTICO DE CONEXIÓN: Rojo
            std::cerr << client::colors::RED 
                      << "Error al conectar: " << e.what() 
                      << client::colors::RESET << std::endl;
            throw;
        }
    }

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
            std::cerr << client::colors::RED 
                      << "[-] Error in post_json_https: " << e.what() 
                      << client::colors::RESET << std::endl;
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

    // Subir datos multipart HTTPS (para push upload)
    nlohmann::json upload_push_data(const std::string &path,  const std::string &repoName, const std::string &userEmail, const std::string &userPassword, const std::string &operationsJsonStr, const std::string &tar_filepath) {
        try {
            auto cli = conect();
            
            // Read binary TAR file
            std::ifstream file(tar_filepath, std::ios::binary | std::ios::ate);
            if (!file) throw std::runtime_error("\n[!] Could not read the .tar file");
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<char> buffer(size);
            if (!file.read(buffer.data(), size)) throw std::runtime_error("\n[-] Error: reading tar bytes");
            std::string tar_content(buffer.begin(), buffer.end());

            std::string dynamicFileName = repoName + ".tar.gz";


            std::vector<httplib::MultipartFormData> items = {
                // Operations JSON
                {"operations", operationsJsonStr, "", "application/json"},
                
                // TAR File
                {"tarFile", tar_content, dynamicFileName, "application/x-tar"},
                
                // Credentials and Repo (Plain text)
                {"repoName", repoName, "", ""},
                {"userEmail", userEmail, "", ""},
                {"userPassword", userPassword, "", ""}
            };

            auto res = cli->Post(path.c_str(), items);
            
            if (!res) throw std::runtime_error("\n[!] No response from server");
            if (res->status != 200) {
                // Try to return server error if it is JSON
                try { return nlohmann::json::parse(res->body); } 
                catch(...) { throw std::runtime_error("Server error: " + res->body); }
            }
            
            return nlohmann::json::parse(res->body);

        } catch (const std::exception &e) {
            std::cerr << client::colors::RED 
                      << "\n[!] Upload error: " << std::endl << "     " << e.what() 
                      << client::colors::RESET << std::endl;
            throw;
        }
    }

    // Enviar peticiones get HTTPS
    nlohmann::json get_json_https(const std::string &path) {
        try {
            auto cli = conect(); 
            cli->set_read_timeout(10, 0);

            // GET
            auto res = cli->Get(path.c_str());

            // Error de Conexión
            if (!res) {
                return {
                    {"status", "error"},
                    {"message", "[!] Could not contact the server (Connection failed)"}
                };
            }

            // Verificar Status HTTP
            if (res->status == 200) {
                try {
                    return nlohmann::json::parse(res->body);
                } catch (...) {
                    return {
                        {"status", "error"},
                        {"message", "Error parsing response JSON: " + res->body}
                    };
                }
            } else {

                // Si el servidor devuelve error (404, 500, etc.)
                return {
                    {"status", "error"},
                    {"message", "HTTP Error " + std::to_string(res->status)}
                };
            }

        } catch (const std::exception &e) {
            std::cerr << client::colors::RED 
                      << "\n[!] Exception in get_json_https: " << e.what() 
                      << client::colors::RESET << std::endl;
            return {
                {"status", "error"},
                {"message", e.what()}
            };
        }
    }

}