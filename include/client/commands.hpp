#pragma once

#include <map> 
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace client::cmd {

    bool run_create_user(const std::string&, const std::string&, const std::string&);

    bool run_init(const std::string&, const std::string&, const std::string&);

    bool run_keygen(const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_clone(const std::string&, const std::string&, const std::string&, const std::string&);
    
    bool run_push(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

    void run_log();

    bool run_verify_user(const std::string&, const std::string&, const std::string&);

    bool run_set_role(const std::string&, const std::string&, const std::string&, int);

    bool run_set_status(const std::string&, const std::string&, const std::string& , int);

    bool run_keygen_rsa(const std::string&, const std::string&, const std::string&);

    bool run_encrypt_repo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
    
    bool run_decrypt_repo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_enroll_user(const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_decrypt_repo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
   
    bool run_grant_access_file(const std::string& repo_name, const std::string& file_path,const std::string& target_email,const std::string& senior_email, const std::string& password);

    void run_list_repos();

    void run_list_encrypted(const std::string& email, const std::string& password);

    void run_list_accessible(const std::string& email, const std::string& password);

    void run_list_files(const std::string& email, const std::string& password, const std::string& repo_name);

    bool run_get_key(const std::string& email, const std::string& password, const std::string& repo_name, const std::string& repo_alias, const std::string& output_dir);

    
    
}