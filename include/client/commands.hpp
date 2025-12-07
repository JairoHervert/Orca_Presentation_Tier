#pragma once
#include <string>
#include <vector>

namespace client::cmd {

    bool run_nuser(const std::string&, const std::string&, const std::string&);

    bool run_init(const std::string&, const std::string&, const std::string&);

    bool run_keygen(const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_clone(const std::string&, const std::string&, const std::string&, const std::string&);
    
    bool run_push(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_log(const std::string&);

    bool run_revoke(const std::string&, const std::string&, const std::string&);

    bool run_grant(const std::string&, const std::string&, const std::string&);

    bool run_drop(const std::string&);

    bool run_active(const std::string&);

    bool run_remove(const std::string&);

    bool run_verify(const std::string&, const std::string&, const std::string&);

    bool run_change_role(const std::string&, const std::string&, const std::string&, int);

    bool run_change_status(const std::string&, const std::string&, const std::string& , int);

    bool run_keygen_rsa(const std::string&, const std::string&, const std::string&);

    bool run_cypher_repo(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
    
    bool run_dcypher_repo();

    bool run_enroll(const std::string&, const std::string&, const std::string&, const std::string&);

    bool run_uncyp(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
   
}