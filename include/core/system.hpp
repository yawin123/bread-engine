#pragma once

#include <memory>
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
      virtual void Update() = 0;
    };

    class SystemManager
    {
      public:
        explicit SystemManager() = default;

        template<typename T>
        T& AddSystem()
        {
          TypeInfoRef id = typeid(T);

          T* ret = GetSystem<T>();
          if(ret) return *ret;

          auto ptr = std::make_unique<T>();
          ret = ptr.get();

          systems[id] = std::move(ptr);
          return *ret;
        }

        template<typename T>
        T* GetSystem() const
        {
          auto it = systems.find(typeid(T));

          if(it != systems.end())
          {
            return reinterpret_cast<T*>(it->second.get());
          }

          return nullptr;
        }

      private:
        hash_map<System> systems;
    };
  };
};
