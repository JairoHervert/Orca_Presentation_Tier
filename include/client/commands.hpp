#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

namespace client::cmd {

    bool run_nuser(const std::string&, const std::string&, const std::string&);

    bool run_init(const std::string&, const std::string&, const std::string&);

    bool run_keygen(const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_clone(const std::string&, const std::string&, const std::string&, const std::string&);
    
    bool run_push(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

    void run_log();

    bool run_verify(const std::string&, const std::string&, const std::string&);

    bool run_change_role(const std::string&, const std::string&, const std::string&, int);

    bool run_change_status(const std::string&, const std::string&, const std::string& , int);

    bool run_keygen_rsa(const std::string&, const std::string&, const std::string&);

    bool run_cypher_repo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
    
    bool run_dcypher_repo();

    bool run_enroll(const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_uncyp(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
   
    bool run_add_user_file(const std::string& repo_name, const std::string& file_path,const std::string& target_email,const std::string& senior_email, const std::string& password);

}