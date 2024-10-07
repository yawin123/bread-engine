#pragma once

#include <BreadEngine/core/components.hpp>
#include <BreadEngine/core/context.hpp>
#include <BreadEngine/core/utils.hpp>

namespace brd
{
  class Script : public core::Component
  {
    public:
      explicit Script(core::brdID entityID);

      virtual void Start(core::Context& ctxt) {}
      virtual void Update(core::Context& ctxt) {}
  };
};
