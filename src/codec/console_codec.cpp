#include "client/console_codec.hpp"
namespace client::console {

    std::string get_password_secure(const std::string& prompt) {
        std::string password;
        std::cout << prompt;

    #ifdef _WIN32
        // --- WINDOWS (con asteriscos) ---
        char ch;
        while ((ch = _getch()) != 13) {
            if (ch == 8) {
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                std::cout << "*";
            }
        }
        std::cout << std::endl;
    #else
        // --- LINUX (sin echo) ---
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        std::getline(std::cin, password);
        
        // Restaurar terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::cout << std::endl;
    #endif

        return password;
    }


    // Expresion Regular 
    bool validate_password_policy(const std::string& password) {
        std::regex pattern("^(?=.*[A-Za-z])(?=.*\\d).{8,}$");
        return std::regex_match(password, pattern);
    }
}