// g++ pruebajson.cpp -o pruebajson && ./pruebajson
#include <iostream>
#include <string>
#include "../third_party/json.hpp"

int main() {
    using nlohmann::json;

    // Construir un objeto JSON
    json j{
        {"ok", true},
        {"version_check", "nlohmann/json"},
        {"numbers", {1, 2, 3}},
        {"nested", {{"key", "value"}}}
    };

    // Serializar (compacto)
    std::cout << "Compact: " << j.dump() << "\n";

    // Serializar (formateado)
    std::cout << "Pretty:\n" << j.dump(2) << "\n";

    // Parsear desde string para verificar ida y vuelta
    std::string s = R"({"hello":"world","answer":42})";
    json parsed = json::parse(s);

    std::cout << std::endl;
    std::cout << std::endl;

    // recuperar solo el valor asociado a version_check
    std::cout << "a ver: " << j["version_check"] << "\n";

    return 0;
}
