#pragma once

#include <BreadEngine/core/type_traits.hpp>
#include <memory>

extern "C" {
  #include <GLFW/glfw3.h>
}

namespace brd
{
  using Vector2 = std::tuple<float&,float&>;
  using constVector2 = const std::tuple<const float&, const float&>;

  class Sprite;
  class Transform2D;
  class ScriptWrapper;
  using ComponentManagerType = brd::core::type_traits::type_list<Sprite, Transform2D, ScriptWrapper>;
};
