#pragma once

#include <memory>

extern "C" {
  #include <GLFW/glfw3.h>
}

namespace brd
{
  using Vector2 = std::tuple<float&,float&>;
  using constVector2 = const std::tuple<const float&, const float&>;
};
