#pragma once

#include <BreadEngine/core/components.hpp>
#include <BreadEngine/core/entities.hpp>
#include <BreadEngine/core/slotmap.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/core/utils.hpp>

#include <BreadEngine/typedeclarations.hpp>

namespace brd
{
  namespace core
  {
    class Context
    {
      public:
        explicit Context() = default;

        //template<typename... CMPS>
        inline Entity& CreateEntity()
        {
          Entity& e = entities[entities.push(Entity{})];
          //(... AddComponent<CMPS>(e) );
          return e;
        }

        template<typename T, typename... InitTypes>
        T& AddComponent(Entity& entity, InitTypes&&... initVals)
        {
          return components.CreateComponent<T>(entity.id, initVals...);
        }

        template<typename T>
        slotmap<T>& GetComponents()
        {
          return components.GetComponents<T>();
        }

      private:
        slotmap<Entity> entities;
        ComponentManager<brd::ComponentManagerType> components;
    };
  };
};
