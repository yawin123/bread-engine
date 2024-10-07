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

        explicit Entity() = default;
        virtual ~Entity() = default;

        brdID id;
        mask_type cmp_mask {};

        template<typename CMP>
        constexpr inline bool hasComponent() const
        {
          return (cmp_mask & mask_info::mask<CMP>());
        }

        template<typename CMP>
        void addComponent(brdID cmpID)
        {
          cmp_mask |= mask_info::mask<CMP>();
          pAddComponent<CMP>(cmpID);
        }

        template<typename CMP>
        std::optional<brdID> getComponent(brdID cmpID = 0)
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
        std::vector<brdID>& getComponents()
        {
          constexpr auto cmptypeid { CMTinfo::template id<CMP>() };
          return std::get<cmptypeid>(componentData);
        }

      private:
        using lista_a_indices = type_traits::foreach_make_replace<brdID,ComponentManagerType>;
        using indices_a_vectores = type_traits::foreach_make_container<std::vector, lista_a_indices>;
        using ComponentDataType = type_traits::make_container<std::tuple, indices_a_vectores>;

        ComponentDataType componentData;


        template<typename CMP>
        requires(!IsSingleComponent<CMP>) // Se ejecuta si T no hereda de SingleComponent
        void pAddComponent(brdID cmpID)
        {
          auto& data = getComponents<CMP>();
          data.push_back(cmpID);
        }

        template<typename CMP>
        requires(IsSingleComponent<CMP>) // Se ejecuta si T hereda de SingleComponent
        void pAddComponent(brdID cmpID)
        {
          auto& data = getComponents<CMP>();
          data.clear();
          data.push_back(cmpID);
        }
    };
  };
};
