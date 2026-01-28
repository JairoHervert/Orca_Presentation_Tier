# Orca — Presentation Tier (Front / CLI)

Este repositorio contiene la **capa de presentación (Front)** del proyecto **Orca**: un **cliente CLI (Command Line Interface)** escrito en **C++** que se comunica con el backend mediante **HTTPS**.

---

## ¿Qué es Orca?

**Orca** es un sistema orientado a **proteger archivos de código fuente** (y, en general, contenido de repositorios) mediante mecanismos criptográficos, permitiendo:

- **Cifrar y descifrar repositorios** para proteger su contenido.
- **Delimitar el acceso** a un conjunto de **N desarrolladores** (control de acceso), de modo que solo usuarios autorizados puedan obtener llaves y/o acceder al contenido.
- **Administrar usuarios, roles y estatus**, así como flujos de enrolamiento y concesión de acceso.
- Interactuar con el sistema mediante comandos, manteniendo el uso desde terminal como una interfaz reproducible y automatizable.

> **Nota**: Esta capa es el **cliente**. La lógica de persistencia, validación y autorización definitiva vive en el **backend**.

---

## Backend (Application Tier)

Repositorio del backend (placeholder, por agregar):

- **Orca Backend (Application Tier)**: **`<PENDIENTE: URL_DEL_REPO_BACKEND>`**

---

## Tecnologías y dependencias

- **C++17**
- **OpenSSL** (`-lssl -lcrypto`) para HTTPS/criptografía en la capa de transporte.
- **Crypto++** (`-lcryptopp`) para primitivas criptográficas utilizadas por el cliente.
- **CLI11** (framework para CLI):  
  https://github.com/CLIUtils/CLI11
- **dotenv-cpp** (carga de variables de entorno desde `.env`):  
  https://github.com/laserpants/dotenv-cpp
- **nlohmann/json** (JSON):  
  https://json.nlohmann.me/

---

## Compilación en Linux

### Requisitos

Asegúrate de contar con:
- `g++` con soporte **C++17**
- OpenSSL (headers + libs)
- Crypto++ (headers + libs)

En Debian/Ubuntu suele ser algo como:
```bash
sudo apt-get install build-essential libssl-dev libcrypto++-dev
```

(En Fedora los nombres de paquetes pueden variar.)

### Comando de compilación

```bash
g++ -std=c++17 -I include/third_party -I include \
  src/cli/main_cli.cpp \
  src/app/commands/init.cpp \
  src/app/commands/clone.cpp \
  src/app/commands/push.cpp \
  src/app/commands/create_user.cpp \
  src/app/commands/log.cpp \
  src/app/commands/keygen.cpp \
  src/codec/json_codec.cpp \
  src/codec/crypto/decipher_RSA_codec.cpp \
  src/codec/crypto/decipher_AES_codec.cpp \
  src/codec/crypto/generate_keypair_codec.cpp \
  src/codec/console_codec.cpp \
  src/codec/files_codec.cpp \
  src/transport/client_https.cpp \
  src/app/responses_handlers/init_handler.cpp \
  src/app/responses_handlers/clone_handler.cpp \
  src/app/responses_handlers/push_handler.cpp \
  src/app/responses_handlers/create_user_handler.cpp \
  src/app/responses_handlers/log_handler.cpp \
  src/app/responses_handlers/keygen_ecdsa_handler.cpp \
  src/app/responses_handlers/keygen_rsa_handler.cpp \
  src/codec/downloader_codec.cpp \
  src/transport/http_getter.cpp \
  src/codec/unpacker_codec.cpp \
  src/codec/scanner_codec.cpp \
  src/codec/comparator_codec.cpp \
  src/codec/packer_codec.cpp \
  src/codec/crypto/hasher_codec.cpp \
  src/app/commands/verify_user.cpp \
  src/app/responses_handlers/verify_user_handler.cpp \
  src/app/commands/set_role.cpp \
  src/app/responses_handlers/set_role_handler.cpp \
  src/app/commands/set_status.cpp \
  src/app/responses_handlers/set_status_handler.cpp \
  src/app/commands/encrypt_repo.cpp \
  src/app/responses_handlers/encrypt_repo_handler.cpp \
  src/app/commands/enroll_user.cpp \
  src/app/responses_handlers/enroll_user_handler.cpp \
  src/app/commands/decrypt_repo.cpp \
  src/app/responses_handlers/decrypt_repo_handler.cpp \
  src/codec/crypto/sign_codec.cpp \
  src/app/responses_handlers/grant_access_handler.cpp \
  src/app/commands/grant_access.cpp \
  src/app/responses_handlers/commits_handler.cpp \
  src/app/commands/list_repos.cpp \
  src/app/responses_handlers/list_repos_handler.cpp \
  src/app/commands/list_encrypted.cpp \
  src/app/responses_handlers/list_encrypted_handler.cpp \
  src/app/commands/list_accessible.cpp \
  src/app/responses_handlers/list_accessible_handler.cpp \
  src/app/responses_handlers/list_files_handler.cpp \
  src/app/commands/list_files.cpp \
  src/app/commands/get_key.cpp \
  src/app/responses_handlers/get_key_handler.cpp \
  -o orca -lssl -lcrypto -lcryptopp -pthread
```

---

## Configuración

### Variables de entorno

Crea un archivo `.env` en la raíz del proyecto con las siguientes variables:

```env
HOST=localhost
PORT=8443
```

Estas variables definen el **host** y **puerto** del backend con el que se comunicará el cliente CLI.

### Workspace

Por defecto, Orca utiliza el directorio `./workspace` para almacenar configuraciones locales, claves y repositorios. Asegúrate de que este directorio tenga los permisos adecuados.

---

## Uso

### Ayuda general (CLI11)

```bash
./orca --help
```

### Ayuda por comando

```bash
./orca <comando> --help
```

---

## Ejemplos de comandos

Los siguientes ejemplos son orientativos y pueden requerir ajustar flags/argumentos según la implementación exacta del CLI.

### Inicializar configuración / workspace

```bash
./orca init --help
./orca init
```

### Crear usuario

```bash
./orca create_user --help
./orca create_user --name "Alice" --email "alice@correo.com" --password "123456"
```

### Verificar usuario / sesión

```bash
./orca verify_user --help
./orca verify_user --email "alice@correo.com" --token "<TOKEN>"
```

### Enrolar usuario

```bash
./orca enroll_user --help
./orca enroll_user --email "bob@correo.com"
```

### Establecer rol de usuario

```bash
./orca set_role --help
./orca set_role --email "alice@correo.com" --role "admin"
```

### Establecer estatus de usuario

```bash
./orca set_status --help
./orca set_status --email "alice@correo.com" --status "active"
```

### Generar par de llaves

```bash
./orca keygen --help
./orca keygen --type rsa --email "alice@correo.com"
```

### Cifrar repositorio

```bash
./orca encrypt_repo --help
./orca encrypt_repo --path "./mi-repo" --name "proyecto-secreto"
```

### Descifrar repositorio

```bash
./orca decrypt_repo --help
./orca decrypt_repo --name "proyecto-secreto" --output "./mi-repo-descifrado"
```

### Conceder acceso a repositorio

```bash
./orca grant_access --help
./orca grant_access --repo "proyecto-secreto" --email "bob@correo.com"
```

### Obtener llave de repositorio

```bash
./orca get_key --help
./orca get_key --repo "proyecto-secreto"
```

### Listar repositorios

```bash
./orca list_repos --help
./orca list_repos
```

### Listar repositorios cifrados

```bash
./orca list_encrypted --help
./orca list_encrypted
```

### Listar repositorios accesibles

```bash
./orca list_accessible --help
./orca list_accessible --email "alice@correo.com"
```

### Listar archivos en un repositorio

```bash
./orca list_files --help
./orca list_files --repo "proyecto-secreto"
```

### Clonar repositorio

```bash
./orca clone --help
./orca clone --name "proyecto-secreto" --output "./clon-local"
```

### Push de cambios

```bash
./orca push --help
./orca push --repo "proyecto-secreto" --message "Actualización de módulo X"
```

### Ver log de commits

```bash
./orca log --help
./orca log --repo "proyecto-secreto"
```

---

## Estructura del proyecto

```
.
├── include/
│   ├── third_party/     # Headers de librerías externas
│   └── ...              # Headers propios del proyecto
├── src/
│   ├── cli/             # Punto de entrada (main_cli.cpp)
│   ├── app/
│   │   ├── commands/    # Implementación de comandos CLI
│   │   └── responses_handlers/  # Manejadores de respuestas del backend
│   ├── codec/           # Codecs (JSON, crypto, archivos, etc.)
│   ├── transport/       # Cliente HTTPS y HTTP getter
│   └── ...
├── workspace/           # Directorio de trabajo local (gitignored)
├── .env                 # Variables de entorno (gitignored)
└── README.md
```