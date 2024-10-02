#pragma once

#include <BreadEngine/core/slotmap.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/core/utils.hpp>

#include <tuple>
#include <utility>

namespace brd
{
  namespace core
  {
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

    template<typename CL>
    class ComponentManager
    {
      public:
        using componentlist = CL;
        using componentinfo = type_traits::traits<componentlist>;

        explicit ComponentManager() = default;

        template<typename T, typename... InitTypes>
        T& CreateComponent(brdID entityID, InitTypes&&... initVals)
        {
          auto& components = GetComponents<T>();
          auto& component = components[components.push(T{entityID, std::forward<InitTypes>(initVals)...})];
          return component;
        }

        template<typename T>
        slotmap<T>& GetComponents()
        {
          constexpr auto id { componentinfo::template id<T>() };

          return std::get<id>(data);
        }

      private:
        //hash_map<Container> data;

        template<typename T>
        using container = slotmap<T>;

        using DataContainer = type_traits::make_container<std::tuple, type_traits::foreach_make_container<container,componentlist>>;
        DataContainer data;
    };
  };
};
