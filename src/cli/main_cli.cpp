// Estando en Orca_Presentation_Tier/
// g++ -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/codec/json_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp -o orca -lssl -lcrypto

//Compilacion Karol:
// cd C:/Users/kgonz/Desktop/OrcaProject/Orca_Presentation_Tier
// g++ -D_WIN32_WINNT=0x0A_00 -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/codec/json_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/core/downloader.cpp src/transport/http_getter.cpp src/app/core/unpacker.cpp -o orca -lssl -lcrypto -lws2_32 -lcrypt32

// si en windows usan otro comando ponerlo aqui (no modificar el que ya funciona en linux)
#include <iostream>
#include "CLI11.hpp"

// Handlers de subcomandos
#include "client/commands.hpp"

int main(int argc, char** argv) {
   CLI::App app{"Orca Presentation Tier CLI"};

   // --- Subcomando: init ---
   auto* init = app.add_subcommand("init", "Inicializa un nuevo repositorio remoto");
   std::string repo_name;
   init->add_option("-n,--name", repo_name, "Nombre")->required();
   std::vector<std::string> collaborators;
   init->add_option("-c,--collaborators", collaborators, "Lista de colaboradores");

   // --- Subcomando: clone ---
   auto* clone = app.add_subcommand("clone", "Clona un repositorio remoto");
   std::string repo_pname;
   clone->add_option("-n,--name", repo_pname, "Nombre del repositorio")->required();
   std::string destination;
   clone->add_option("-d, --dir", destination, "Directorio de destino")->default_val("./")->required();

   // Parsear los argumentos
   CLI11_PARSE(app, argc, argv);

   // Ejecutar el subcomando correspondiente
   if (init->parsed()) client::cmd::run_init(repo_name, collaborators);
   if (clone->parsed()) client::cmd::run_clone(repo_pname, destination);


   // Si no se ejecuta algun subcomando, muestra ayuda
   if (app.get_subcommands().empty()) {
      std::cout << app.help() << std::endl;
   }

   return 0;
}
