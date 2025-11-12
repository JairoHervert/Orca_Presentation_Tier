// Aqui pondriamos varias librerias ya que este archivo es el puente con el cpp de las respuestas del servidor
#pragma once
#include <iostream>
#include "../third_party/json.hpp"

namespace client::response_handler {

   void handle_init_response(const nlohmann::json &response);

   // Otras funciones de manejo de respuestas pueden ir aquí

}