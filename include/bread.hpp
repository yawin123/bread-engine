#pragma once

#include <BreadEngine/core/context.hpp>
#include <BreadEngine/core/internal_api.hpp>
#include <BreadEngine/core/system.hpp>
#include <BreadEngine/core/slotmap.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/core/utils.hpp>

namespace brd {
  namespace core {
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

      core::Context& GetContext()
      {
        return context;
      }

      template<typename T>
      T& GetSystem()
      {
        return system.template GetSystem<T>();
      }

      template<typename T>
      void ConfigureSystem(core::SystemConfiguration& config)
      {
        GetSystem<T>().Configure(config);
      }

      template<typename T>
      T& GetComponent(core::brdID cmpID)
      {
        return context.GetComponent<T>(cmpID);
      }

      template<typename T>
      std::optional<T*> GetComponent(brd::core::Entity& entity, core::brdID ecmpID = 0)
      {
        auto& cmpID = entity.getComponent<T>(ecmpID);
        return (cmpID.has_value()) ? context.GetComponent<T>(cmpID.value()) : nullptr;
      }

    private:
      SystemManagerType system;
      brd::core::Context context;
      brd::core::InternalApi internalApi;
  };
};
