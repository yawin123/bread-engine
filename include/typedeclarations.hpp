#pragma once

#include <memory>

extern "C" {
  #include <GLFW/glfw3.h>
}

namespace brd
{
  struct GLFWWindowDeleter {
    void operator()(GLFWwindow* window) {
        glfwDestroyWindow(window);
    }
  };

  using Window_ptr = std::unique_ptr<GLFWwindow,GLFWWindowDeleter>();

  using Vector2 = std::tuple<float&,float&>;
  using constVector2 = const std::tuple<const float&, const float&>;
};
