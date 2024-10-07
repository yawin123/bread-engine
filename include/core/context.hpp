#pragma once

#include <BreadEngine/core/components.hpp>
#include <BreadEngine/core/entities.hpp>
#include <BreadEngine/core/slotmap.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/core/utils.hpp>

#include <BreadEngine/typedeclarations.hpp>

#include <iostream>

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
          const auto& e = entities.push(Entity{});
          auto& entity = GetEntity(e);
          entity.id = e;

          //(... AddComponent<CMPS>(e) );
          return entity;
        }

        inline Entity& GetEntity(const brdID& entityID)
        {
          auto& e = entities[entityID];
          return e;
        }

        template<typename T, typename... InitTypes>
        requires(!IsSingleComponent<T>) // Se ejecuta si T no hereda de SingleComponent
        T& AddComponent(Entity& entity, InitTypes&&... initVals)
        {
          auto rCMP = components.CreateComponent<T>(entity.id, initVals...);
          entity.addComponent<T>(rCMP);
          return components.GetComponents<T>()[rCMP];
        }

        template<typename T, typename... InitTypes>
        requires(IsSingleComponent<T>) // Se ejecuta si T hereda de SingleComponent
        T& AddComponent(Entity& entity, InitTypes&&... initVals)
        {
          if(!entity.hasComponent<T>())
          {
            auto rCMP = components.CreateComponent<T>(entity.id, initVals...);
            entity.addComponent<T>(rCMP);
            return components.GetComponents<T>()[rCMP];
          }
          else
          {
            return components.GetComponents<T>()[entity.getComponent<T>().value()];
          }
        }

        template<typename T>
        slotmap<T>& GetComponents()
        {
          return components.GetComponents<T>();
        }

        template<typename T>
        T& GetComponent(brdID cmpID)
        {
          return components.GetComponent<T>(cmpID);
        }

      private:
        slotmap<Entity> entities;
        ComponentManager<brd::ComponentManagerType> components;
    };
  };
};
