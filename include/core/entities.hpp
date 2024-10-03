#pragma once

#include <BreadEngine/core/slotmap.hpp>
#include <BreadEngine/core/utils.hpp>
#include <BreadEngine/core/type_traits.hpp>
#include <BreadEngine/typedeclarations.hpp>

#include <optional>

namespace brd
{
  namespace core
  {
    class Entity
    {
      public:
        using mask_info = type_traits::traits<ComponentManagerType>;
        using mask_type = typename mask_info::mask_type;
        using CMTinfo = type_traits::traits<ComponentManagerType>;

        explicit Entity() : id(++nextID){}
        virtual ~Entity() = default;

        const brdID id;
        mask_type cmp_mask {};

        template<typename CMP>
        constexpr inline bool hasComponent() const
        {
          return (cmp_mask & mask_info::mask<CMP>());
        }

        template<typename CMP>
        void addComponent(index_type cmpID)
        {
          cmp_mask |= mask_info::mask<CMP>();
          pAddComponent<CMP>(cmpID);
        }

        template<typename CMP>
        std::optional<index_type> getComponent(index_type cmpID = 0)
        {
          if(hasComponent<CMP>())
          {
            //Obtenemos el contenedor
              auto& data = getComponents<CMP>();

            //Si el elemento solicitado está
              return (data.size() > cmpID) ? data[cmpID] : data[0];
          }

          return {};
        }

        template<typename CMP>
        std::vector<index_type>& getComponents()
        {
          constexpr auto cmptypeid { CMTinfo::template id<CMP>() };
          return std::get<cmptypeid>(componentData);
        }

      private:
        inline static brdID nextID {0};

        using lista_a_indices = type_traits::foreach_make_replace<index_type,ComponentManagerType>;
        using indices_a_vectores = type_traits::foreach_make_container<std::vector, lista_a_indices>;
        using ComponentDataType = type_traits::make_container<std::tuple, indices_a_vectores>;

        ComponentDataType componentData;


        template<typename CMP>
        requires(!IsSingleComponent<CMP>) // Se ejecuta si T no hereda de SingleComponent
        void pAddComponent(index_type cmpID)
        {
          auto& data = getComponents<CMP>();
          data.push_back(cmpID);
        }

        template<typename CMP>
        requires(IsSingleComponent<CMP>) // Se ejecuta si T hereda de SingleComponent
        void pAddComponent(index_type cmpID)
        {
          auto& data = getComponents<CMP>();
          data.clear();
          data.push_back(cmpID);
        }
    };
  };
};
