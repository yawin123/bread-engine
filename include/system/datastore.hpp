#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#include <BreadEngine/core/internal_api.hpp>
#include <BreadEngine/core/system.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/typedeclarations.hpp>

namespace brd
{
  struct DataStoreConfiguration : public core::SystemConfiguration
  {
    std::string dataDir {"."};
    std::string saveFile {"savedata"};
    std::string saveExtension {"json"};
  };

  template<typename... Ts>
  class DataStore : public core::System
  {
    private:
      using datalist = core::type_traits::type_list<Ts...>;
      using datainfo = core::type_traits::traits<datalist>;

    public:
      explicit DataStore()
      {
        // Registrar callbacks para distintos tipos, aquí un ejemplo para 'int', 'float', 'std::string'
        (RegisterCallbacks<Ts>(), ...);
      }

      virtual ~DataStore() = default;

      virtual void Configure(core::SystemConfiguration& conf) noexcept override
      {
        configuration = static_cast<DataStoreConfiguration&>(conf);
      }
      virtual void Update(core::Context& ctxt) noexcept {}

      template<typename T>
      T& Get(const std::string& key)
      {
        constexpr auto id { datainfo::template id<T>() };
        return std::get<id>(data)[key];
      }

      template<typename T>
      void Set(const std::string& key, T&& value)
      {
        constexpr auto id { datainfo::template id<T>() };
        std::get<id>(data)[key] = value;
      }

      void Save()
      {
        nlohmann::json j;

        //Generamos el json a guardar
        SaveDataList<datalist>(j);

        // Guardar el JSON en un archivo
        std::ofstream file(configuration.dataDir + "/" + configuration.saveFile + "." + configuration.saveExtension);
        if (file.is_open())
        {
          file << j.dump(4); // Formato bonito con 4 espacios de indentación
          file.close();
        }
        else
        {
          std::cerr << "No se pudo abrir el archivo para guardar los datos.\n";
        }
      }

      void Load()
      {
        // Leer el archivo JSON
        std::ifstream file(configuration.dataDir + "/" + configuration.saveFile + "." + configuration.saveExtension);
        if (file.is_open())
        {
          nlohmann::json j;
          file >> j;
          file.close();
          LoadDataList<datalist>(j);
        }
        else
        {
          std::cerr << "No se pudo abrir el archivo para cargar los datos.\n";
        }
      }

    private:
      template<typename T>
      using container = std::unordered_map<std::string,T>;
      using containers = core::type_traits::foreach_make_container<container, datalist>;

      core::type_traits::make_container<std::tuple, containers> data;
      DataStoreConfiguration configuration;

      template<typename T>
      void SaveData(nlohmann::json& j)
      {
        constexpr auto id { datainfo::template id<T>() };
        const auto& dataMap = std::get<id>(data);

        nlohmann::json dataJson;
        for(const auto& [key, value] : dataMap)
        {
          dataJson[key] = value;
        }

        j[std::to_string(id)] = dataJson;
      }

      template<typename TL>
      void SaveDataList(nlohmann::json& j)
      {
        if constexpr (TL::size() > 0)
        {
          SaveData<core::type_traits::head<TL>>(j);

          if constexpr (TL::size() > 1)
          {
            SaveDataList<core::type_traits::pop<TL>>(j);
          }
        }
      }

      template<typename T>
      void LoadData(nlohmann::json& j)
      {
        constexpr auto id_ { datainfo::template id<T>() };

        std::string id = std::to_string(id_);

        for(const auto& [key, value] : j[id].items())
        {
          Set<T>(key, static_cast<T>(value));
        }
      }

      template<typename TL>
      void LoadDataList(nlohmann::json& j)
      {
        if constexpr (TL::size() > 0)
        {
          LoadData<core::type_traits::head<TL>>(j);

          if constexpr (TL::size() > 1)
          {
            LoadDataList<core::type_traits::pop<TL>>(j);
          }
        }
      }

      template<typename T>
      void RegisterCallbacks()
      {
        // Convertimos la lambda explícitamente a std::function
        core::InternalApi::AddCallback<T, std::string>(
          "datastore.get",
          std::function<T(std::string)>{
            [this](std::string key) -> T {
              return this->Get<T>(key); // Llamada a la función Get del DataStore
            }
          }
        );

        core::InternalApi::AddCallback<void, std::string, T&&>(
          "datastore.set",
          std::function<void(std::string, T&&)>{
            [this](std::string key, T&& value) {
              this->Set(key, std::forward<T>(value)); // Usamos std::move para optimización
            }
          }
        );
      }
  };

  namespace Data
  {
    template<typename T>
    std::optional<T> Get(const std::string& key) {
      return brd::core::InternalApi::Call<T, std::string>("datastore.get", std::string(key));
    }

    template<typename T>
    void Set(const std::string& key, T&& value) {
      brd::core::InternalApi::Call<void, std::string, T&&>("datastore.set", std::string(key), std::forward<T>(value));
    }
  };
};
