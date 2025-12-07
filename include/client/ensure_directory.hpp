#include <filesystem>

namespace client::ensure_directory {

    bool ensure_directory(const std::filesystem::path& dir) {
        namespace fs = std::filesystem;

        try {
            // Si existe, confirmar que sea directorio
            if (fs::exists(dir))
                return fs::is_directory(dir);

            // Validar que el padre exista
            fs::path parent = dir.parent_path();
            if (!parent.empty() && !fs::exists(parent))
                return false;

            // Crear el directorio
            fs::create_directories(dir);
            return true;
        }
        catch (...) {
            return false;
        }
    }

}
