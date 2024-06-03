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

        using systemlist = type_traits::type_list<SL...>;
        using systeminfo = type_traits::traits<systemlist>;
        template<typename SYSTEM>
        SYSTEM& GetSystem()
        {
          constexpr auto id { systeminfo::template id<SYSTEM>() };
          return std::get<id>(systems);
        }

      private:
        type_traits::make_container<std::tuple, systemlist> systems;
    };
  };
};
