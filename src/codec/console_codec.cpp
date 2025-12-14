#include "client/console_codec.hpp"
#include "client/colors.hpp" // Incluimos colores

namespace client::console {

    std::string get_password_secure(const std::string& prompt) {
        std::string password;
        
        // Coloreamos el Prompt:Negrita
        std::cout << client::colors::BOLD << prompt << client::colors::RESET;

    #ifdef _WIN32
        // --- WINDOWS (con asteriscos) ---
        char ch;
        while ((ch = _getch()) != 13) { // 13 es ENTER
            if (ch == 8) { // 8 es BACKSPACE
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                // Asteriscos en Amarillo
                std::cout << client::colors::BOLD << "*" << client::colors::RESET;
            }
        }
        std::cout << std::endl;
    #else
        // --- LINUX (sin echo) ---
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO; // Desactivar eco
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        std::getline(std::cin, password);
        
        // Restaurar terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::cout << std::endl;
    #endif

        return password;
    }


    // Expresion Regular (No imprime nada, así que no lleva colores)
    bool validate_password_policy(const std::string& password) {
        // Mínimo 8 caracteres, al menos una letra y un número
        std::regex pattern("^(?=.*[A-Za-z])(?=.*\\d).{8,}$");
        return std::regex_match(password, pattern);
    }
}