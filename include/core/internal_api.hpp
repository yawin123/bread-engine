#pragma once

#include <BreadEngine/core/utils.hpp>

#include <functional>

namespace brd
{
  namespace core
  {
    struct CBContainer { virtual ~CBContainer() = default; };

    template<typename FTYPE>
    struct CallbackContainer : public CBContainer
    {
      virtual ~CallbackContainer() = default;
      std::unordered_map<std::string_view,std::function<FTYPE>> callbacks {};
    };

    class InternalApi
    {
      public:
        template<typename R, typename... CallTypes>
        static void AddCallback(std::string_view key, std::function<R(CallTypes...)> callback)
        {
          using FTYPE = R(CallTypes...);

          CallbackContainer<FTYPE>* container {nullptr};
          auto& id = typeid(FTYPE);

          auto it = callbacks.find(id);
          if(it == callbacks.end())
          {
            auto ptr = std::make_unique<CallbackContainer<FTYPE>>();
            container = ptr.get();
            callbacks[id] = std::move(ptr);
          }
          else
          {
            container = reinterpret_cast<CallbackContainer<FTYPE>*>(it->second.get());
          }

          auto cbit = container->callbacks.find(key);
          if(cbit == container->callbacks.end())
          {
            container->callbacks[key] = std::move(callback);
          }
        }

        // Sobrecarga para el caso en que R es void
        template<typename R = void, typename... CallTypes>
        requires (is_void<R>)
        static void Call(std::string_view key, CallTypes&&... callValues)
        {
          using FTYPE = void(CallTypes...);
          CallbackContainer<FTYPE>* container {nullptr};

          auto& id = typeid(FTYPE);

          auto it = callbacks.find(id);
          if(it != callbacks.end())
          {
            container = reinterpret_cast<CallbackContainer<FTYPE>*>(it->second.get());

            auto cbit = container->callbacks.find(key);
            if(cbit != container->callbacks.end())
            {
              // Llamar al callback sin retorno
              cbit->second(std::forward<CallTypes>(callValues)...);
            }
          }
        }

        template<typename R, typename... CallTypes>
        requires (!is_void<R>)
        static std::optional<R> Call(std::string_view key, CallTypes&&... callValues)
        {
          using FTYPE = R(CallTypes...);
          CallbackContainer<FTYPE>* container {nullptr};
          auto& id = typeid(FTYPE);

          auto it = callbacks.find(id);
          if(it != callbacks.end())
          {
            container = reinterpret_cast<CallbackContainer<FTYPE>*>(it->second.get());

            auto cbit = container->callbacks.find(key);
            if(cbit != container->callbacks.end())
            {
              return cbit->second(std::forward<CallTypes>(callValues)...);
            }
          }

          return std::nullopt;
        }


      private:
        inline static hash_map<CBContainer> callbacks {};
    };
  };
};
