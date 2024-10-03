#pragma once

#include <BreadEngine/core/components.hpp>
#include <BreadEngine/core/utils.hpp>
#include <BreadEngine/typedeclarations.hpp>

#include <memory>
#include <string_view>

extern "C" {
  #include <glad.h>
}

namespace brd
{
  class Sprite : public core::SingleComponent
  {
    public:
      explicit Sprite(core::brdID entityID, std::string_view path = "");

      void LoadImg(std::string_view path) noexcept;

      int w {0}, h {0}, chan {0};
      GLuint texture_id;

      unsigned int VAO, VBO, EBO;

      GLfloat vertices[20] = {
        // Posiciones          // Coordenadas de textura
        0.5f,  0.5f, 0.0f,    1.0f, 0.0f, // Arriba derecha
        0.5f, -0.5f, 0.0f,    1.0f, 1.0f, // Abajo derecha
       -0.5f, -0.5f, 0.0f,    0.0f, 1.0f, // Abajo izquierda
       -0.5f,  0.5f, 0.0f,    0.0f, 0.0f, // Arriba izquierda
      };

      // Índices para definir los triángulos
      unsigned int indices[6] = {
          0, 1, 3, // Primer triángulo
          1, 2, 3  // Segundo triángulo
      };
  };
};
