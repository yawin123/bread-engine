#include <BreadEngine/core/context.hpp>

namespace brd
{
  namespace core
  {
    Context::Context()
    {
      entities.reserve(INITIALVECTORSIZE);
    }

    Entity& Context::CreateEntity()
    {
      return entities.emplace_back();
    }
  };
};
