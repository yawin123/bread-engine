#pragma once

#include <BreadEngine/core/slotmap.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/core/utils.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace brd
{
  namespace core
  {
    class Component
    {
      public:
        explicit Component(brdID eID_);

        constexpr brdID GetId() const noexcept { return id; }

      protected:
        const brdID eID {0};
        const brdID id {0};
        inline static brdID nextID {0};
    };

    class SingleComponent : public Component
    {
      public:
        explicit SingleComponent(brdID eID_);
    };

    template<typename T>
    concept IsSingleComponent = std::is_base_of_v<SingleComponent, T>;


    template<typename CL>
    class ComponentManager
    {
      public:
        using componentlist = CL;
        using componentinfo = type_traits::traits<componentlist>;

        explicit ComponentManager() = default;

        template<typename T, typename... InitTypes>
        const brdID CreateComponent(brdID entityID, InitTypes&&... initVals)
        {
          auto& components = GetComponents<T>();
          const auto component = components.push(T{entityID, std::forward<InitTypes>(initVals)...});
          return component;
        }

        template<typename T>
        slotmap<T>& GetComponents()
        {
          constexpr auto id { componentinfo::template id<T>() };

          return std::get<id>(data);
        }

        template<typename T>
        T& GetComponent(brdID componentID)
        {
          slotmap<T>& d = GetComponents<T>();
          return d[componentID];
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
