// Estando en Orca_Presentation_Tier/
// g++ -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/codec/json_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp -o orca -lssl -lcrypto

//Compilacion Karol:
// cd C:/Users/kgonz/Desktop/OrcaProject/Orca_Presentation_Tier
// g++ -D_WIN32_WINNT=0x0A00 -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/app/commands/push.cpp src/app/commands/config.cpp src/app/commands/log.cpp src/codec/json_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/responses_handlers/push_handler.cpp src/app/responses_handlers/config_handler.cpp src/app/responses_handlers/log_handler.cpp src/codec/downloader.cpp src/transport/http_getter.cpp src/codec/unpacker.cpp src/codec/hasher.cpp src/codec/comparator.cpp src/codec/packer.cpp -o orca -lssl -lcrypto -lws2_32 -lcrypt32 -lcryptopp

// si en windows usan otro comando ponerlo aqui (no modificar el que ya funciona en linux)
#include <iostream>
#include "CLI11.hpp"

// Handlers de subcomandos
#include "client/commands.hpp"

int main(int argc, char** argv) {
   CLI::App app{"Orca Presentation Tier CLI"};

   // --- VARIABLES COMPARTIDAS ---
   std::string repo_name;   
   std::string working_dir;
    
   // --- Subcomando config ---
    auto* config = app.add_subcommand("config", "Configura usuario y email global");
    std::string user_name;
    config->add_option("-u,--user", user_name, "Nombre de usuario")->required();
    std::string user_email;
    config->add_option("-e,--email", user_email, "Correo electronico")->required();

   // --- Subcomando: init ---
   auto* init = app.add_subcommand("init", "Inicializa un nuevo repositorio remoto");
   init->add_option("-n,--name", repo_name, "Nombre")->required();
   std::vector<std::string> collaborators;
   init->add_option("-c,--collaborators", collaborators, "Lista de colaboradores");

   // --- Subcomando: clone ---
   auto* clone = app.add_subcommand("clone", "Clona un repositorio remoto");

   clone->add_option("-n,--name", repo_name, "Nombre del repositorio")->required();
   clone->add_option("-d, --dir", working_dir, "Directorio de destino")->default_val("./")->required();

   // --- Subcomando: push
   auto* push = app.add_subcommand("push", "Sube los cambios de un proyecto al Repositorio Remoto");
   push->add_option("-n,--name", repo_name, "Nombre del proyecto en el servidor")->required();
   push->add_option("-d,--dir", working_dir, "Directorio local del proyecto")->default_val("./");
  
   // --- Subcomando: log
   auto* log = app.add_subcommand("log", "Muestra el historial de cambios");
   log->add_option("-n,--name", repo_name, "Nombre del proyecto")->required();

   // Parsear los argumentos
   CLI11_PARSE(app, argc, argv);

   // Ejecutar el subcomando correspondiente
   if (init->parsed()) client::cmd::run_init(repo_name, collaborators);
   if (clone->parsed()) {
      std::string absolute_path = std::filesystem::absolute(working_dir).string();
      client::cmd::run_clone(repo_name, absolute_path);
   }
   if (push->parsed()) {
      std::string absolute_path = std::filesystem::absolute(working_dir).string();
      client::cmd::run_push(repo_name, absolute_path);
   }
   if (config->parsed()) {
        client::cmd::run_config(user_name, user_email);
   }

   if (log->parsed()) {
        client::cmd::run_log(repo_name);
    }

   // Si no se ejecuta algun subcomando, muestra ayuda
   if (app.get_subcommands().empty()) {
      std::cout << app.help() << std::endl;
   }

   return 0;
}
