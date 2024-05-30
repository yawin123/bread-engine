#pragma once

#include <memory>
#include <cstdint>
#include <string>

#include <BreadEngine/typedeclarations.hpp>
#include <BreadEngine/core/system.hpp>

extern "C" {
  #include <glad.h>
}

namespace brd
{
  struct RenderConfiguration : public core::SystemConfiguration
  {
    int width {0}, height {0};
    std::string title {""};
  };

  class Render : public core::System
  {
    public:
      explicit Render();
      virtual ~Render() = default;

      virtual void Configure(core::SystemConfiguration& conf) noexcept override;
      virtual void Update(core::Context& ctxt) noexcept;

      bool ShouldWindowClose() const noexcept;

    private:
      using GLFWDestructor = void(*)(GLFWwindow*);
      using Window_ptr = std::unique_ptr<GLFWwindow, GLFWDestructor>;

      static void gLFWWindowDeleter(GLFWwindow* window);

      RenderConfiguration configuration;

      Window_ptr window{glfwCreateWindow(0,0,"",nullptr, nullptr),
                        gLFWWindowDeleter};
      //GLFWwindow* window;

      // Vertex Shader
        const char* vertexShaderSource = R"(
            #version 400 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;

            out vec2 TexCoord;

            uniform float pos_x;
            uniform float pos_y;
            uniform float scale_x;
            uniform float scale_y;

            void main() {
                gl_Position = vec4((aPos.x + pos_x) * scale_x,
                                   (aPos.y + pos_y) * scale_x,
                                   aPos.z, 1.0);
                TexCoord = aTexCoord;
            }
        )";

      // Fragment Shader
        const char* fragmentShaderSource = R"(
            #version 400 core
            out vec4 FragColor;

            in vec2 TexCoord;

            uniform sampler2D ourTexture; // Textura que vamos a mapear

            void main() {
                FragColor = texture(ourTexture, TexCoord);
            }
        )";

      GLuint shaderProgram;
      GLuint posxID, posyID, scalxID, scalyID;
  };
};
