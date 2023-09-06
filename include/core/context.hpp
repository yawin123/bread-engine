#pragma once

#include <BreadEngine/core/components.hpp>
#include <BreadEngine/core/entities.hpp>
#include <BreadEngine/core/utils.hpp>

namespace brd
{
  namespace core
  {
    class Context
    {
      public:
        explicit Context();
        virtual ~Context() = default;

        Entity& CreateEntity();

        template<typename T>
        T& AddComponent(Entity& entity)
        {
          return components.CreateComponent<T>(entity.id);
        }

        template<typename T>
        ComponentContainer<T>& GetComponents()
        {
          return components.GetComponents<T>();
        }

      private:
        std::vector<Entity> entities;
        ComponentManager components;
    };
  };
};
