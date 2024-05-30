#include <BreadEngine/system/render.hpp>
#include <BreadEngine/components/sprite.hpp>
#include <BreadEngine/components/transform.hpp>

extern "C" {
  #include <GLFW/glfw3.h>
}

#include <iostream>

namespace brd
{
  void Render::gLFWWindowDeleter(GLFWwindow* window)
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  Render::Render()
  {
    // Inicializa GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << "\n";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }

  void Render::Configure(core::SystemConfiguration& conf)
  {
    configuration = reinterpret_cast<RenderConfiguration&>(conf);

    // Se modifica la ventana
      window = Window_ptr{glfwCreateWindow(configuration.width,
                                           configuration.height,
                                           configuration.title.c_str(),
                                           nullptr, nullptr),
                          gLFWWindowDeleter};

    // Establece el contexto actual de OpenGL
      glfwMakeContextCurrent(window.get());

    // Inicializa glad para cargar funciones de OpenGL
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar glad" << "\n";
      }

    // Configura el Viewport
      glViewport(0, 0, configuration.width, configuration.height);

    // Compila y enlaza los shaders
      // Crea el shader de vértice
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

      // Crea el shader de fragmento
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

      // Crea el programa de shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

      // Libera los shaders, ya que no se necesitan después del enlace
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

      // Usa el programa de shaders
        glUseProgram(shaderProgram);

      //Configuramos las variables de transformación para el vertex shader
        posxID = glGetUniformLocation(shaderProgram, "pos_x");
        posyID = glGetUniformLocation(shaderProgram, "pos_y");
        scalxID = glGetUniformLocation(shaderProgram, "scale_x");
        scalyID = glGetUniformLocation(shaderProgram, "scale_y");

      // Establecemos el color de fondo
        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
  }

  void Render::Update(core::Context& ctxt)
  {
    // Limpia el búfer de color
      glClear(GL_COLOR_BUFFER_BIT);

    // Usamos los shaders
      glUseProgram(shaderProgram);

    auto& sprites = ctxt.GetComponents<Sprite>();
    auto& transforms = ctxt.GetComponents<Transform2D>();

    // Aquí puedes dibujar tu imagen o realizar cualquier otra operación de renderizado
    for(int i=0; i < sprites.data.size(); ++i)
    {
      glBindTexture(GL_TEXTURE_2D, sprites.data[i].texture_id); //Activamos la textura

      // Transformación
        auto p = transforms.data[i].Position();
        glUniform1f(posxID, std::get<0>(p));
        glUniform1f(posyID, std::get<1>(p));

        auto s = transforms.data[i].Scale();
        glUniform1f(scalxID, std::get<0>(s));
        glUniform1f(scalyID, std::get<1>(s));

      // Dibuja el cuadrado
        glBindVertexArray(sprites.data[i].VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // Intercambia los búferes frontal y trasero
      glfwSwapBuffers(window.get());
  }

  bool Render::ShouldWindowClose() const
  {
    glfwPollEvents();
    return glfwWindowShouldClose(window.get());
  }
};
