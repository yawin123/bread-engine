#include <BreadEngine/components/transform.hpp>

namespace brd
{
  Transform2D::Transform2D(core::brdID entityID) : core::Component(entityID) {}

  constVector2 Transform2D::Position() const
  {
    return {p_x, p_y};
  }

  constVector2 Transform2D::Scale() const
  {
    return {s_x, s_y};
  }

  const float& Transform2D::Rotation() const
  {
    return r;
  }

  Vector2 Transform2D::Position()
  {
    return {p_x, p_y};
  }

  Vector2 Transform2D::Scale()
  {
    return {s_x, s_y};
  }

  float& Transform2D::Rotation()
  {
    return r;
  }
};
