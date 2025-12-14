// Estando en Orca_Presentation_Tier/
// g++ -I include/third_party -I include src/cli/main_cli.cpp ... (tus comandos de compilación)

// Estando en Orca_Presentation_Tier/
// g++ -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/app/commands/push.cpp src/app/commands/create_user.cpp src/app/commands/log.cpp src/app/commands/keygen_ecdsa.cpp src/codec/json_codec.cpp src/codec/files_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/responses_handlers/push_handler.cpp src/app/responses_handlers/create_user_handler.cpp src/app/responses_handlers/log_handler.cpp src/app/responses_handlers/keygen_ecdsa_handler.cpp src/codec/downloader.cpp src/transport/http_getter.cpp src/codec/unpacker.cpp src/codec/hasher.cpp src/codec/comparator.cpp src/codec/packer.cpp -o orca -lssl -lcrypto -lcryptopp

//Compilacion Karol:
// cd C:/Users/kgonz/Desktop/OrcaProject/Orca_Presentation_Tier
// g++ -D_WIN32_WINNT=0x0A00 -I include/third_party -I include src/cli/main_cli.cpp src/app/commands/init.cpp src/app/commands/clone.cpp src/app/commands/push.cpp src/app/commands/create_user.cpp src/app/commands/log.cpp src/app/commands/keygen.cpp src/codec/json_codec.cpp src/codec/Crypto/decipher_RSA_codec.cpp src/codec/Crypto/decipher_AES_codec.cpp src/codec/Crypto/generate_keypair_codec.cpp src/codec/console_codec.cpp src/codec/files_codec.cpp src/transport/client_https.cpp src/app/responses_handlers/init_handler.cpp src/app/responses_handlers/clone_handler.cpp src/app/responses_handlers/push_handler.cpp src/app/responses_handlers/create_user_handler.cpp src/app/responses_handlers/log_handler.cpp src/app/responses_handlers/keygen_ecdsa_handler.cpp src/app/responses_handlers/keygen_rsa_handler.cpp src/codec/downloader_codec.cpp src/transport/http_getter.cpp src/codec/unpacker_codec.cpp src/codec/scanner_codec.cpp src/codec/comparator_codec.cpp src/codec/packer_codec.cpp src/codec/Crypto/hasher_codec.cpp src/app/commands/verify_user.cpp src/app/responses_handlers/verify_user_handler.cpp src/app/commands/set_role.cpp src/app/responses_handlers/set_role_handler.cpp src/app/commands/set_status.cpp src/app/responses_handlers/set_status_handler.cpp src/app/commands/encrypt_repo.cpp src/app/responses_handlers/encrypt_repo_handler.cpp src/app/commands/enroll_user.cpp src/app/responses_handlers/enroll_user_handler.cpp src/app/commands/decrypt_repo.cpp src/app/responses_handlers/decrypt_repo_handler.cpp src/codec/Crypto/sign_codec.cpp  src/app/responses_handlers/grant_access_handler.cpp src/app/commands/grant_access.cpp src/app/responses_handlers/commits_handler.cpp src/app/commands/list_repos.cpp src/app/responses_handlers/list_repos_handler.cpp src/app/commands/list_encrypted.cpp src/app/responses_handlers/list_encrypted_handler.cpp src/app/commands/list_accessible.cpp src/app/responses_handlers/list_accessible_handler.cpp src/app/responses_handlers/list_files_handler.cpp src/app/commands/list_files.cpp src/app/commands/get_key.cpp src/app/responses_handlers/get_key_handler.cpp -o orca -lssl -lcrypto -lws2_32 -lcrypt32 -lcryptopp


#include <iostream>
#include "CLI11.hpp"

#include <iostream>
#include <filesystem> 
#include "CLI11.hpp"

// Handlers de subcomandos
#include "client/colors.hpp"
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

   
   // --- Subcomando create_user --- //
   auto* create_user = app.add_subcommand("create_user", "Registers a new user in the system");
   create_user->add_option("-u,--user", user_name, "User name")->required();
   create_user->add_option("-e,--email", user_email, "User email address")->required();

   // --- Subcomando: init --- //
   auto* init = app.add_subcommand("init", "Initializes a new remote repository");
   init->add_option("-n,--name", repo_name, "Repository Name")->required();
   init->add_option("-e,--email", user_email, "Creator's email")->required();

      // --- Subcomando: keygen --- //
   auto* keygen = app.add_subcommand("keygen", "Generates a cryptographic key pair");
   std::string keyType;
   keygen->add_option("-t,--type", keyType, "Key type (e.g., ecdsa, aes)")->required();
   keygen->add_option("-e,--email", user_email, "User email associated with the account")->required();
   keygen->add_option("-d,--dir,--dtt", working_dir, "Existing directory to save the private key")->default_val("./");

   // --- Subcomando: clone --- //
   auto* clone = app.add_subcommand("clone", "Clones a remote repository");
   clone->add_option("-n,--name", repo_name, "Repository Name")->required();
   clone->add_option("-e,--email", user_email, "User email")->required();
   clone->add_option("-d,--dir,--dtt", working_dir, "Destination directory")->default_val("./");

      // --- Subcomando: verify_user --- //
   auto* verify_user = app.add_subcommand("verify_user", "Verifies (approves) a new user");
   std::string approver_email;
   verify_user->add_option("-a,--approver", approver_email, "Approver email (Senior)")->required();
   verify_user->add_option("-t,--target", user_email, "Target user email to verify")->required();
   
      // --- Subcomando: set_role --- //
   auto* set_role = app.add_subcommand("set_role", "Changes a user's role level");
   set_role->add_option("-a,--approver", approver_email, "Approver email (Senior)")->required();
   set_role->add_option("-t,--target", user_email, "Target user email")->required();
   int new_role = 0;
   set_role->add_option("-r,--role", new_role, "New role (1=Dev, 2=Leader, 3=Senior)")->required();

    // --- Subcomando: set_status --- //
    auto* set_status = app.add_subcommand("set_status", "Changes a user's status");
    set_status->add_option("-a,--approver", approver_email, "Approver email (Senior)")->required();
    set_status->add_option("-t,--target", user_email, "Target user email")->required();
    int new_status = 0;
    set_status->add_option("-s,--status", new_status, "New status (1=Active, 0=Inactive)")->required();
   
    // --- COMANDO: enroll_user --- //
    auto* enroll_user = app.add_subcommand("enroll_user", "Adds a user to an existing project");
    enroll_user->add_option("-n,--name", repo_name, "Project Name")->required();
    enroll_user->add_option("-a,--approver", approver_email, "Leader/Senior email")->required();
    enroll_user->add_option("-t,--target", user_email, "User email to enroll")->required();

    // --- COMANDO: encrypt_repo --- //
    auto* encrypt_repo = app.add_subcommand("encrypt_repo", "Encrypts a repository on the server");
    encrypt_repo->add_option("-n,--name", repo_name, "Repository Name")->required();
    std::string repo_tag;
    encrypt_repo->add_option("-t,--tag", repo_tag, "Tag or Version")->required();
    std::string senior_email;
    encrypt_repo->add_option("-l,--leader", user_email, "Leader email")->required(); 
    encrypt_repo->add_option("-s,--senior", senior_email, "Senior email")->required();

    // --- COMANDO: decrypt_repo --- //
    auto* decrypt_repo = app.add_subcommand("decrypt_repo", "Downloads and decrypts a protected repository");
    decrypt_repo->add_option("-n,--name", repo_name, "Repository Name")->required();
    decrypt_repo->add_option("-t,--tag", repo_tag, "Tag or Version")->required();
    decrypt_repo->add_option("-d,--dir, --dtt", working_dir, "Destination directory")->default_val("./");
    decrypt_repo->add_option("-e,--email", user_email, "User email (owner or senior)")->required();
    decrypt_repo->add_option("-k,--key", keyPath, "Directory containing AES.key")->default_val("./");

    // --- Subcomando: grant_access --- //
    auto* grant_access = app.add_subcommand("grant_access", "Grants user permission for a specific file");
    std::string file_path;
    grant_access->add_option("-n,--name", repo_name, "Repository Name")->required();
    grant_access->add_option("-f,--file", file_path, "Relative file path (e.g., src/main.cpp)")->required();
    grant_access->add_option("-a,--approver", approver_email, "Leader/Senior email")->required();
    grant_access->add_option("-t,--target", user_email, "Target user email")->required();
   
    // --- Subcomando: log --- //
    auto* log = app.add_subcommand("log", "Shows the server commit history");

    // --- NUEVO SUBCOMANDO: list ---
    auto* list = app.add_subcommand("list", "Lists all available repositories");

    // --- NUEVO SUBCOMANDO: list-enc ---
    auto* list_enc = app.add_subcommand("list-enc", "Lists encrypted repositories (requires credentials)");
    list_enc->add_option("-e,--email", user_email, "User email")->required();

    // --- NUEVO SUBCOMANDO: list-access ---
    auto* list_access = app.add_subcommand("list-access", "Lists repositories you have access to");
    list_access->add_option("-e,--email", user_email, "User email")->required();

    // --- NUEVO SUBCOMANDO: list-files ---
    auto* list_files = app.add_subcommand("list-files", "Lists accessible files in a repository");
    list_files->add_option("-n,--name", repo_name, "Repository Name")->required();
    list_files->add_option("-e,--email", user_email, "User email")->required();

   // --- Subcomando: push
   auto* push = app.add_subcommand("push", "Uploads project changes to the Remote Repository");
   push->add_option("-n,--name", repo_name, "Project name on server")->required();
   push->add_option("-e,--email", user_email, "User email")->required();
   push->add_option("-d,--dir,--dtt", working_dir, "Local project directory")->default_val("./");
   push->add_option("-k,--key", keyPath, "Path to keys folder (.key)")->default_val("./");

   // --- NUEVO SUBCOMANDO: get-key ---
    auto* get_key = app.add_subcommand("get-key", "Retrieves the AES key of an encrypted repository");
    get_key->add_option("-e,--email", user_email, "User email")->required();
    get_key->add_option("-n,--name", repo_name, "Repository Name")->required();
    get_key->add_option("-t,--tag", repo_tag, "Encryption Alias or Tag")->required();
   get_key->add_option("-d,--dir,--dtt", working_dir, "Directory to save the AES key")->default_val("./");


   // Parsear los argumentos
   CLI11_PARSE(app, argc, argv);

   // Comandos que requieren seguridad
   std::vector<CLI::App*> secure_cmds = {create_user, get_key, list_files, list_access, list_enc, grant_access, decrypt_repo, push, keygen, push, verify_user, set_role, set_status, encrypt_repo, enroll_user, clone, init};
    
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
               std::cerr << "\n" << client::colors::RED << "[!] Weak password. Requirements:" << client::colors::RESET << std::endl;
               std::cerr << client::colors::YELLOW << "  - Minimum 8 characters" << client::colors::RESET << std::endl;
               std::cerr << client::colors::YELLOW << "  - At least one letter" << client::colors::RESET << std::endl;
               std::cerr << client::colors::YELLOW << "  - At least one number" << client::colors::RESET << std::endl;
               
               if (attempts < MAX_ATTEMPTS) {
                   std::cerr << "\n" << client::colors::CYAN << "Remaining attempts: " << (MAX_ATTEMPTS - attempts) << client::colors::RESET << std::endl;
               } else {
                   std::cerr << "\n" << client::colors::RED << "[!] Too many failed attempts." << client::colors::RESET << std::endl;
                   return 1;
               }
           }
        }
   }

    // Ejecutar el subcomando correspondiente
    if (create_user->parsed()) client::cmd::run_create_user(user_name, user_email, password);
    if (init->parsed()) client::cmd::run_init(repo_name, user_email, password);
    if (keygen->parsed()) client::cmd::run_keygen(keyType, working_dir, user_email, password);
    if (clone->parsed()) {
        std::string absolute_path = std::filesystem::absolute(working_dir).string();
        client::cmd::run_clone(repo_name, absolute_path, user_email, password);
    }
    if (verify_user->parsed()) client::cmd::run_verify_user(approver_email, password, user_email);
    if (set_role->parsed()) client::cmd::run_set_role(approver_email, password, user_email, new_role);
    if (set_status->parsed()) client::cmd::run_set_status(approver_email, password, user_email, new_status);
    if (enroll_user->parsed()) client::cmd::run_enroll_user(approver_email, password, repo_name, user_email);
    if (encrypt_repo->parsed()) client::cmd::run_encrypt_repo(user_email, password, senior_email, repo_name, repo_tag);
    if (decrypt_repo->parsed()) {
        std::string absolute_dest = std::filesystem::absolute(working_dir).string();
        std::string absolute_key  = std::filesystem::absolute(keyPath).string();
        client::cmd::run_decrypt_repo(repo_name, repo_tag, absolute_dest, user_email, password, absolute_key);
    }
    if (grant_access->parsed()) client::cmd::run_grant_access_file(repo_name, file_path, user_email, approver_email, password);
    if (log->parsed()) client::cmd::run_log();
    if (list->parsed()) client::cmd::run_list_repos(); 
    if (list_enc->parsed()) client::cmd::run_list_encrypted(user_email, password);
    if (list_access->parsed()) client::cmd::run_list_accessible(user_email, password);
    if (list_files->parsed()) client::cmd::run_list_files(user_email, password, repo_name);
    if (get_key->parsed()) client::cmd::run_get_key(user_email, password, repo_name, repo_tag, working_dir);
    if (push->parsed()) {
      std::string absolute_dest = std::filesystem::absolute(working_dir).string();
      std::string absolute_key  = std::filesystem::absolute(keyPath).string(); 
      client::cmd::run_push(repo_name, user_email, absolute_dest, absolute_key, password);
   }


   // Si no se ejecuta algun subcomando, muestra ayuda
   if (app.get_subcommands().empty()) {
      std::cout << app.help() << std::endl;
   }

   return 0;
}