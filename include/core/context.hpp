#pragma once

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
          
        }

      private:
        std::vector<Entity> entities;
        ComponentManager components;
    };
  };
};
