#include <BreadEngine/core/components.hpp>

namespace brd
{
  namespace core
  {
    Component::Component(brdID eID_) : eID(eID_), id(++nextID) {}
  };
};
