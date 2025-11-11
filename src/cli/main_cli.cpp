// Estando en Orca_Presentation_Tier/
// g++ -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/codec/json_codec.cpp -o orca
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
   std::string repo_url;
   clone->add_option("url", repo_url, "URL del repositorio")->required();
   std::string destination;
   clone->add_option("destination", destination, "Directorio de destino")->default_val("./");

   // Parsear los argumentos
   CLI11_PARSE(app, argc, argv);

   // Ejecutar el subcomando correspondiente
   if (init->parsed()) client::cmd::run_init(repo_name, collaborators);
   if (clone->parsed()) client::cmd::run_clone(repo_url, destination);


   // Si no se ejecuta algun subcomando, muestra ayuda
   if (app.get_subcommands().empty()) {
      std::cout << app.help() << std::endl;
   }

   return 0;
}
