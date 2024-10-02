#pragma once

#include <BreadEngine/core/context.hpp>
#include <BreadEngine/core/slotmap.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/core/utils.hpp>

namespace brd {
  namespace core {
    template<typename SL>
    class SystemManager;

    template<typename CL>
    class ComponentManager;

    class Context;
  };

  template<typename SL, typename CL>
  class Bread
  {
    public:
      using ComponentList = CL;
      using SystemList = SL;

      using ComponentManagerType = brd::core::ComponentManager<ComponentList>;
      using SystemManagerType = brd::core::SystemManager<SystemList>;

      explicit Bread() = default;

      private:
        SystemManagerType system;
        brd::core::Context context;
    };
};
