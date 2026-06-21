#pragma once

namespace Radiant::StyleSheetParser {
  template<typename T>
  class IValue {
    public:
      virtual T resolve() = 0;
  };
}
