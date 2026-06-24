#pragma once

#include <cstdint>
namespace Radiant::StyleSheetParser {
  template<typename T>
  class IValue {
    public:
      virtual T resolve(float referenceLength) = 0;
  };
}
