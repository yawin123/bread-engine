#pragma once

#include <BreadEngine/core/components.hpp>
#include <BreadEngine/core/utils.hpp>
#include <BreadEngine/typedeclarations.hpp>

namespace brd
{
  class Transform2D : public core::SingleComponent
  {
    public:
      explicit Transform2D(core::brdID entityID, float x_p = 0.0f, float y_p = 0.0f, float x_s = 1.0f, float y_s = 1.0f, float r_ = 0.0f);
      virtual ~Transform2D() = default;

      constVector2 Position() const;
      constVector2 Scale() const;
      const float& Rotation() const;

      Vector2 Position();
      Vector2 Scale();
      float& Rotation();

    private:
      float p_x {0.0f}, p_y {0.0f};
      float s_x {1.0f}, s_y {1.0f};
      float r {0.0f};
  };
};
