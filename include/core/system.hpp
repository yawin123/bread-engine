#pragma once

#include <iostream>
#include <memory>
#include <tuple>
#include <BreadEngine/core/context.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/core/utils.hpp>

namespace brd
{
  namespace core
  {
    struct SystemConfiguration
    {
      virtual ~SystemConfiguration() = default;
    };

    struct System
    {
      virtual ~System() = default;
      virtual void Configure(SystemConfiguration& conf) = 0;
      virtual void Update(Context& ctxt) = 0;
    };

    template<typename... SL>
    class SystemManager
    {
      public:
        explicit SystemManager() = default;

        using SystemList = type_traits::Typelist<SL...>;
        using systeminfo = type_traits::TypeTraits<SystemList>;
        template<typename SYSTEM>
        SYSTEM& GetSystem()
        {
          constexpr auto id { systeminfo::template id<SYSTEM>() };
          return std::get<id>(systems);
        }

      private:
        type_traits::replace<std::tuple, SystemList> systems;
    };
  };
};
