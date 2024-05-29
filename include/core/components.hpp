#pragma once

#include <BreadEngine/core/utils.hpp>
#include <BreadEngine/core/slotmap.hpp>

namespace brd
{
  namespace core
  {
    struct Container { virtual ~Container() = default; };

    template<typename T>
    struct ComponentContainer : public Container
    {
      virtual ~ComponentContainer() = default;
      slotmap<T> data;
    };

    class Component
    {
      public:
        explicit Component(brdID eID_);
        virtual ~Component() = default;

        constexpr brdID GetId() const noexcept { return id; }

      private:
        const brdID eID {0};
        const brdID id {0};
        inline static brdID nextID {0};
    };

    class ComponentManager
    {
      public:
        explicit ComponentManager() = default;

        template<typename T>
        T& CreateComponent(brdID entityID)
        {
          auto& components = GetComponents<T>();
          auto& component = components.data.push(entityID);
          return component;
        }

        template<typename T>
        ComponentContainer<T>& GetComponents()
        {
          ComponentContainer<T>* ret {nullptr};
          auto& id = typeid(T);

          auto it = data.find(id);
          if(it != data.end())
          {
            ret = reinterpret_cast<ComponentContainer<T>*>(it->second.get());
          }
          else
          {
            auto ptr = std::make_unique<ComponentContainer<T>>();
            ret = ptr.get();
            data[id] = std::move(ptr);
          }

          return *ret;
        }

      private:
        hash_map<Container> data;
    };
  };
};
