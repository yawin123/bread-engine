#pragma once

#include <cstdlib>
#include <functional>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <concepts>

#define INITIALVECTORSIZE 100

namespace brd
{
  namespace core
  {
    //brdID
      using brdID = std::size_t;

    //TypeInfoRef
      using TypeInfoRef = std::reference_wrapper<const std::type_info>;

    //Hash map
      struct Hasher { std::size_t operator()(TypeInfoRef code) const { return code.get().hash_code(); } };
      struct EqualTo { bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const { return lhs.get() == rhs.get(); } };

      template<typename T>
      using hash_map = std::unordered_map<TypeInfoRef, std::unique_ptr<T>, Hasher, EqualTo>;

    // Concepto para determinar si un tipo es void
      template<typename T>
      concept is_void = std::is_void_v<T>;

    void seetype(auto) { std::cout << __PRETTY_FUNCTION__ << "\n"; }
  };
};
