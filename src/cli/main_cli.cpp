// Estando en Orca_Presentation_Tier/
// g++ -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/app/commands/push.cpp src/app/commands/nuser.cpp src/app/commands/log.cpp src/app/commands/keygen_ecdsa.cpp src/codec/json_codec.cpp src/codec/files_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/responses_handlers/push_handler.cpp src/app/responses_handlers/nuser_handler.cpp src/app/responses_handlers/log_handler.cpp src/app/responses_handlers/keygen_ecdsa_handler.cpp src/codec/downloader.cpp src/transport/http_getter.cpp src/codec/unpacker.cpp src/codec/hasher.cpp src/codec/comparator.cpp src/codec/packer.cpp -o orca -lssl -lcrypto -lcryptopp

//Compilacion Karol:
// cd C:/Users/kgonz/Desktop/OrcaProject/Orca_Presentation_Tier
// g++ -D_WIN32_WINNT=0x0A00 -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/app/commands/push.cpp src/app/commands/nuser.cpp src/app/commands/log.cpp src/app/commands/keygen.cpp src/codec/json_codec.cpp src/codec/decipher_RSA_codec.cpp src/codec/decipher_AES_codec.cpp src/codec/generate_keypair_codec.cpp src/codec/console_codec.cpp src/codec/files_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/responses_handlers/push_handler.cpp src/app/responses_handlers/nuser_handler.cpp src/app/responses_handlers/log_handler.cpp src/app/responses_handlers/keygen_ecdsa_handler.cpp src/app/responses_handlers/keygen_rsa_handler.cpp src/codec/downloader_codec.cpp src/transport/http_getter.cpp src/codec/unpacker_codec.cpp src/codec/scanner_codec.cpp src/codec/comparator_codec.cpp src/codec/packer_codec.cpp src/codec/hasher_codec.cpp src/app/commands/verify.cpp src/app/responses_handlers/verify_handler.cpp src/app/commands/change_role.cpp src/app/responses_handlers/change_role_handler.cpp src/app/commands/change_status.cpp src/app/responses_handlers/change_status_handler.cpp src/app/commands/cypher_repo.cpp src/app/responses_handlers/cypher_repo_handler.cpp src/app/commands/enroll.cpp src/app/responses_handlers/enroll_handler.cpp src/app/commands/uncyp.cpp src/app/responses_handlers/uncyp_handler.cpp src/codec/sign_codec.cpp src/codec/verify_codec.cpp  -o orca -lssl -lcrypto -lws2_32 -lcrypt32 -lcryptopp


// si en windows usan otro comando ponerlo aqui (no modificar el que ya funciona en linux)
#include <iostream>
#include "CLI11.hpp"

// Handlers de subcomandos
#include "client/commands.hpp"
#include "client/console_codec.hpp"

int main(int argc, char** argv) {
   CLI::App app{"Orca Presentation Tier CLI"};

   // --- VARIABLES COMPARTIDAS ---
   std::string repo_name;   
   std::string working_dir;
   std::string user_name;
   std::string user_email;
   std::string target_file; 
   std::string password;
   std::string keyPath;

    
   // --- Subcomando nuser --- //
   auto* nuser = app.add_subcommand("nuser", "Da de alta a un Nuevo usuario en el sistema");
   nuser->add_option("-u,--user", user_name, "Nombre de usuario")->required();
   nuser->add_option("-e,--email", user_email, "Correo electronico")->required();

   // --- Subcomando: init --- //
   auto* init = app.add_subcommand("init", "Inicializa un nuevo repositorio remoto");
   init->add_option("-n,--name", repo_name, "Nombre del Repositorio")->required();
   init->add_option("-e,--email", user_email, "Email del Dueno")->required();

      // --- Subcomando: keygen --- //
   auto* keygen = app.add_subcommand("keygen", "Genera un par de claves criptograficas");
   std::string keyType;
   keygen->add_option("-t,--type", keyType, "Tipo de clave (e.g., ecdsa, aes)")->required();
   keygen->add_option("-o,--output", working_dir, "Directorio existente donde se guardaran la clave privada")->default_val("./");
   keygen->add_option("-e,--email", user_email, "Correo electronico asociado a la cuenta de usuario")->required();

   // --- Subcomando: clone --- //
   auto* clone = app.add_subcommand("clone", "Clona un repositorio remoto");
   clone->add_option("-n,--name", repo_name, "Nombre del repositorio")->required();
   clone->add_option("-e,--email", user_email, "Email del usuario")->required();
   clone->add_option("-d,--dir,--dtt", working_dir, "Directorio de destino")->default_val("./");

      // --- Subcomando: verify --- //
   auto* verify = app.add_subcommand("verify", "Verifica (aprueba) a un usuario nuevo");
   std::string approver_email;
   verify->add_option("-a,--approver", approver_email, "Email del aprobador (Senior)")->required();
   verify->add_option("-t,--target", user_email, "Email del usuario a verificar")->required();
   
      // --- Subcomando: chrole --- //
   auto* chrole = app.add_subcommand("chrole", "Cambia el nivel (rol) de un usuario");
   chrole->add_option("-a,--approver", approver_email, "Email del aprobador (Senior)")->required();
   chrole->add_option("-t,--target", user_email, "Email del usuario a modificar")->required();
   int new_role = 0;
   chrole->add_option("-r,--role", new_role, "Nuevo rol (1=Dev, 2=Leader, 3=Senior)")->required();

    // --- Subcomando: chstatus --- //
    auto* chstatus = app.add_subcommand("chstatus", "Cambia el status de un usuario");
    chstatus->add_option("-a,--approver", approver_email, "Email del aprobador (Senior)")->required();
    chstatus->add_option("-t,--target", user_email, "Email del usuario a modificar")->required();
    int new_status = 0;
    chstatus->add_option("-s,--status", new_status, "Nuevo rol (1=Activo, 0=Inactivo)")->required();
   
    // --- COMANDO: ENROLL ---
    auto* enroll = app.add_subcommand("enroll", "Agrega un usuario a un proyecto existente");
    enroll->add_option("-n,--name", repo_name, "Nombre del proyecto")->required();
    enroll->add_option("-a,--approver", approver_email, "Email del Lider/Senior")->required();
    enroll->add_option("-t,--target", user_email, "Email del usuario a agregar")->required();


   
   
   // --- Subcomando: push
   auto* push = app.add_subcommand("push", "Sube los cambios de un proyecto al Repositorio Remoto");
   push->add_option("-n,--name", repo_name, "Nombre del proyecto en el servidor")->required();
   push->add_option("-e,--email", user_email, "Email del usuario")->required();
   push->add_option("-d,--dir", working_dir, "Directorio local del proyecto")->default_val("./");
   push->add_option("-k,--key", keyPath, "Ruta de la carpeta con las llaves (.key)")->default_val("./");

   // --- Subcomando: log
   auto* log = app.add_subcommand("log", "Muestra el historial de cambios");
   log->add_option("-n,--name", repo_name, "Nombre del proyecto")->required();








   // --- COMANDO: cyprepo ---
   auto* cyprepo = app.add_subcommand("cyprepo", "Cifra un repositorio en el servidor");
   cyprepo->add_option("-n,--name", repo_name, "Nombre del repositorio")->required();
   std::string repo_tag;
   cyprepo->add_option("-t,--tag", repo_tag, "Tag o version")->required();
   std::string senior_email;
   cyprepo->add_option("-l,--leader", user_email, "Email del Lider")->required(); 
   cyprepo->add_option("-s,--senior", senior_email, "Email del Senior")->required();
   cyprepo->add_option("-o,--output", working_dir, "Directorio existente donde se encuentra la clave privada RSA")->default_val("./");

   // --- COMANDO: UNCYP ---
   auto* uncyp = app.add_subcommand("uncyp", "Descarga y descifra un repositorio protegido");
   uncyp->add_option("-n,--name", repo_name, "Nombre del repositorio")->required();
   uncyp->add_option("-d,--dir", working_dir, "Directorio de destino")->default_val("./");
   uncyp->add_option("-e,--email", user_email, "Email del usuario")->required();
   uncyp->add_option("-k,--key", keyPath, "Directorio donde esta la llave AES.key")->default_val("./");


   // Parsear los argumentos
   CLI11_PARSE(app, argc, argv);

   // Comandos que requieren seguridad
   std::vector<CLI::App*> secure_cmds = {nuser, init, uncyp, push, keygen, push, verify, chrole, chstatus, cyprepo, enroll, clone};
    
   bool needs_password = false;
   for (auto* cmd : secure_cmds) {
       if (cmd->parsed()) {
           needs_password = true;
           break;
       }
   }

   if (needs_password) {
       bool valid = false;
       int attempts = 0;
       const int MAX_ATTEMPTS = 3;
       
      while (!valid && attempts < MAX_ATTEMPTS) {
           password = client::console::get_password_secure("Password: ");
           
           if (client::console::validate_password_policy(password)) {
               valid = true;
           } else {
               attempts++;
               std::cerr << "\n[!] Weak password. Requirements:" << std::endl;
               std::cerr << "  - Minimum 8 characters" << std::endl;
               std::cerr << "  - At least one letter" << std::endl;
               std::cerr << "  - At least one number" << std::endl;
               
               if (attempts < MAX_ATTEMPTS) {
                   std::cerr << "\nRemaining attempts: " << (MAX_ATTEMPTS - attempts) << std::endl;
               } else {
                   std::cerr << "\n[!] Too many failed attempts." << std::endl;
                   return 1;
               }
           }
        }
   }

   // Ejecutar el subcomando correspondiente
   if (nuser->parsed()) client::cmd::run_nuser(user_name, user_email, password);
   if (init->parsed()) client::cmd::run_init(repo_name, user_email, password);
   if (keygen->parsed()) client::cmd::run_keygen(keyType, working_dir, user_email, password);
   if (clone->parsed()) {
      std::string absolute_path = std::filesystem::absolute(working_dir).string();
      client::cmd::run_clone(repo_name, absolute_path, user_email, password);
   }
   if (verify->parsed()) client::cmd::run_verify(approver_email, password, user_email);
   if (chrole->parsed()) client::cmd::run_change_role(approver_email, password, user_email, new_role);
   if (chstatus->parsed()) client::cmd::run_change_status(approver_email, password, user_email, new_status);
   if (enroll->parsed()) client::cmd::run_enroll(approver_email, password, repo_name, user_email);


   if (push->parsed()) {
      std::string absolute_dest = std::filesystem::absolute(working_dir).string();
      std::string absolute_key  = std::filesystem::absolute(keyPath).string();
      client::cmd::run_push(repo_name, user_email, absolute_dest, keyPath, password);
   }
   if (log->parsed()) client::cmd::run_log(repo_name);
   if (cyprepo->parsed()) client::cmd::run_cypher_repo(user_email, password, senior_email, repo_name, repo_tag, working_dir);
   if (uncyp->parsed()) {
      std::string absolute_dest = std::filesystem::absolute(working_dir).string();
      std::string absolute_key  = std::filesystem::absolute(keyPath).string();
      client::cmd::run_uncyp(repo_name, absolute_dest, user_email, password, absolute_key);
   }

   // Si no se ejecuta algun subcomando, muestra ayuda
   if (app.get_subcommands().empty()) {
      std::cout << app.help() << std::endl;
   }

   return 0;
}
