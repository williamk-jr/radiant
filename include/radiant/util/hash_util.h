#pragma once

#include <cstddef>
#include <functional>
namespace hash_util {

  template<class T>
  void combineHash(size_t& seed, const T& type) {
    std::hash<T> hasher;
    seed ^= hasher(type) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }
}
