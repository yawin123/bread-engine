#pragma once

#include <BreadEngine/core/utils.hpp>

namespace brd
{
  namespace core
  {
    class Entity
    {
      public:
        explicit Entity() : id(++nextID){}
        virtual ~Entity() = default;

        const brdID id;

      private:
        inline static brdID nextID {0};
    };
  };
};
