#pragma once

namespace Radiant {
  template<typename T>
  class IValue {
    public:
      virtual T resolve() = 0;
  };
}
