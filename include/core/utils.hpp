#pragma once

#include <cstdlib>
#include <functional>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#define INITIALVECTORSIZE 100

namespace brd
{
  namespace core
  {
    //TypeInfoRef
      using TypeInfoRef = std::reference_wrapper<const std::type_info>;

    //Hash map
      struct Hasher { std::size_t operator()(TypeInfoRef code) const { return code.get().hash_code(); } };
      struct EqualTo { bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const { return lhs.get() == rhs.get(); } };

      template<typename T>
      using hash_map = std::unordered_map<TypeInfoRef, std::unique_ptr<T>, Hasher, EqualTo>;

    //brdID
      using brdID = std::size_t;

  };
};
