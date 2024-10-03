#include <BreadEngine/components/sprite.hpp>
#include <BreadEngine/system/render.hpp>

extern "C" {
  #define STB_IMAGE_IMPLEMENTATION
  #include <stb_image.h>

  #include <GLFW/glfw3.h>
}

namespace brd
{
  Sprite::Sprite(core::brdID entityID, std::string_view path) : core::SingleComponent(entityID)
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configura los atributos de vértice
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);

    if(path != "") LoadImg(path);
  }

  void Sprite::LoadImg(std::string_view path)
  {
    std::unique_ptr<unsigned char> data = std::unique_ptr<unsigned char>(stbi_load(path.data(), &w, &h, &chan, STBI_rgb));

    texture_id; // Identificador de la textura
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Configura los parámetros de textura (puedes personalizar estos valores)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Asigna los datos de imagen a la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());

    // Genera mipmaps para una mejor calidad de textura
    glGenerateMipmap(GL_TEXTURE_2D);

  }
};
