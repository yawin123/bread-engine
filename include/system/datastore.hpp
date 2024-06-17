#pragma once

#include <unordered_map>
#include <string>

#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/typedeclarations.hpp>
#include <BreadEngine/core/system.hpp>

namespace brd
{
  struct DataStoreConfiguration : public core::SystemConfiguration
  {
    std::string dataDir {""};
  };

  template<typename... Ts>
  class DataStore : public core::System
  {
    private:
      using datalist = core::type_traits::type_list<Ts...>;
      using datainfo = core::type_traits::traits<datalist>;

    public:
      explicit DataStore() = default;
      virtual ~DataStore() = default;

      virtual void Configure(core::SystemConfiguration& conf) noexcept override
      {
        configuration = static_cast<DataStoreConfiguration&>(conf);
      }
      virtual void Update(core::Context& ctxt) noexcept {}

      template<typename T>
      T& Get(std::string key)
      {
        constexpr auto id { datainfo::template id<T>() };
        return std::get<id>(data)[key];
      }

      template<typename T>
      void Set(std::string key, T&& value)
      {
        constexpr auto id { datainfo::template id<T>() };
        std::get<id>(data)[key] = value;
      }

      void Save()
      {
        for(unsigned int i = 0; i < datainfo::size(); i++)
        {
          using id = core::type_traits::type_from_list<i,Ts...>;
          //SaveData<datainfo::>();
        }
      }

      void Load()
      {

      }

    private:
      template<typename T>
      using container = std::unordered_map<std::string,T>;
      using containers = core::type_traits::foreach_make_container<container, datalist>;

      core::type_traits::make_container<std::tuple, containers> data;
      DataStoreConfiguration configuration;

      template<typename T>
      void SaveData()
      {

      }
  };
};
