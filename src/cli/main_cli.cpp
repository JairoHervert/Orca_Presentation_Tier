// Estando en Orca_Presentation_Tier/
// g++ -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/app/commands/push.cpp src/app/commands/config.cpp src/app/commands/log.cpp src/app/commands/revoke.cpp src/app/commands/grant.cpp src/app/commands/drop.cpp src/app/commands/active.cpp src/app/commands/remove.cpp src/app/commands/keygen_ecdsa.cpp src/codec/json_codec.cpp src/codec/crypto_codec.cpp src/codec/files_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/responses_handlers/push_handler.cpp src/app/responses_handlers/config_handler.cpp src/app/responses_handlers/log_handler.cpp src/app/responses_handlers/revoke_handler.cpp src/app/responses_handlers/grant_handler.cpp src/app/responses_handlers/drop_handler.cpp src/app/responses_handlers/active_handler.cpp src/app/responses_handlers/remove_handler.cpp src/app/responses_handlers/keygen_ecdsa_handler.cpp src/codec/downloader.cpp src/transport/http_getter.cpp src/codec/unpacker.cpp src/codec/hasher.cpp src/codec/comparator.cpp src/codec/packer.cpp -o orca -lssl -lcrypto -lcryptopp

//Compilacion Karol:
// cd C:/Users/kgonz/Desktop/OrcaProject/Orca_Presentation_Tier
// g++ -D_WIN32_WINNT=0x0A00 -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/app/commands/push.cpp src/app/commands/config.cpp src/app/commands/log.cpp src/app/commands/revoke.cpp src/app/commands/grant.cpp src/app/commands/drop.cpp src/app/commands/active.cpp src/app/commands/remove.cpp src/app/commands/keygen_ecdsa.cpp src/codec/json_codec.cpp src/codec/crypto_codec.cpp src/codec/files_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/responses_handlers/push_handler.cpp src/app/responses_handlers/config_handler.cpp src/app/responses_handlers/log_handler.cpp src/app/responses_handlers/revoke_handler.cpp src/app/responses_handlers/grant_handler.cpp src/app/responses_handlers/drop_handler.cpp src/app/responses_handlers/active_handler.cpp src/app/responses_handlers/remove_handler.cpp src/app/responses_handlers/keygen_ecdsa_handler.cpp src/codec/downloader.cpp src/transport/http_getter.cpp src/codec/unpacker.cpp src/codec/hasher.cpp src/codec/comparator.cpp src/codec/packer.cpp -o orca -lssl -lcrypto -lws2_32 -lcrypt32 -lcryptopp

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
   std::string user_name;
   std::string user_email;
   std::string target_file; 
   std::string password;

    
   // --- Subcomando config ---
   auto* config = app.add_subcommand("config", "Configura usuario y email global");
   config->add_option("-u,--user", user_name, "Nombre de usuario")->required();
   config->add_option("-e,--email", user_email, "Correo electronico")->required();

   // --- Subcomando: init ---
   auto* init = app.add_subcommand("init", "Inicializa un nuevo repositorio remoto");
   init->add_option("-n,--name", repo_name, "Nombre")->required();
   std::vector<std::string> collaborators;
   init->add_option("-c,--collaborators", collaborators, "Lista de colaboradores");

      // --- Subcomando: keygen ---
   auto* keygen = app.add_subcommand("keygen", "Genera un par de claves criptográficas");
   std::string keyType;
   keygen->add_option("-t,--type", keyType, "Tipo de clave (e.g., ecdsa, aes)")->required();
   keygen->add_option("-o,--output", working_dir, "Directorio existente donde se guardaran la clave privada")->default_val("./");
   keygen->add_option("-e,--email", user_email, "Correo electrónico asociado a la cuenta de usuario")->required();
   keygen->add_option("-p,--password", password, "Contraseña asociada a la cuenta de usuario")->required();

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

   // --- Subcomando: revoke
   auto* revoke = app.add_subcommand("revoke", "Revoca permisos de acceso");
   revoke->add_option("-n,--name", repo_name, "Nombre del proyecto")->required();
   revoke->add_option("-e,--email", user_email, "Email del usuario a revocar")->required();
   revoke->add_option("-f,--file", target_file, "Archivo especifico (Opcional)");

   // --- Subcomando: grant
   auto* grant = app.add_subcommand("grant", "Concede permisos de acceso");
    grant->add_option("-n,--name", repo_name, "Nombre del proyecto")->required();
    grant->add_option("-e,--email", user_email, "Email del usuario")->required();
    grant->add_option("-f,--file", target_file, "Archivo especifico (Opcional)");

   // --- Subcomando: drop
   auto* drop = app.add_subcommand("drop", "Da de baja a un usuario del sistema");
   drop->add_option("-e,--email", user_email, "Email del usuario a dar de baja")->required();

   // --- Subcomando: active
   auto* active = app.add_subcommand("active", "Reactiva a un usuario del sistema");
   active->add_option("-e,--email", user_email, "Email del usuario a activar")->required();

   // --- Subcomando: remove
   auto* remove = app.add_subcommand("remove", "Elimina un repositorio del servidor");
   remove->add_option("-n,--name", repo_name, "Nombre del repositorio a borrar")->required();


   // Parsear los argumentos
   CLI11_PARSE(app, argc, argv);

   // Ejecutar el subcomando correspondiente
   if (config->parsed()) client::cmd::run_config(user_name, user_email);
   if (init->parsed()) client::cmd::run_init(repo_name, collaborators);
   if (keygen->parsed()) client::cmd::run_keygen(keyType, working_dir, user_email, password);

   if (clone->parsed()) {
      std::string absolute_path = std::filesystem::absolute(working_dir).string();
      client::cmd::run_clone(repo_name, absolute_path);
   }
   if (push->parsed()) {
      std::string absolute_path = std::filesystem::absolute(working_dir).string();
      client::cmd::run_push(repo_name, absolute_path);
   }
   if (log->parsed()) client::cmd::run_log(repo_name);
   if (revoke->parsed()) client::cmd::run_revoke(repo_name, user_email, target_file);
   if (grant->parsed()) client::cmd::run_grant(repo_name, user_email, target_file);
   if (drop->parsed()) client::cmd::run_drop(user_email);
   if (active->parsed()) client::cmd::run_active(user_email);
   if (remove->parsed()) client::cmd::run_remove(repo_name);
    

   // Si no se ejecuta algun subcomando, muestra ayuda
   if (app.get_subcommands().empty()) {
      std::cout << app.help() << std::endl;
   }

   return 0;
}
