#pragma once

#include <BreadEngine/core/components.hpp>
#include <BreadEngine/core/utils.hpp>
#include <BreadEngine/typedeclarations.hpp>

namespace brd
{
  class Sprite : public core::Component
  {
    public:
      explicit Sprite(core::brdID entityID);
      virtual ~Sprite() = default;

      Surface_ptr graph;
  };
};
