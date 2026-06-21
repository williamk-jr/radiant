#pragma once

#include <cstdint>
namespace Radiant {
  class Integer {
    public:
      Integer();
      Integer(int32_t value);

      operator int32_t() const {
        return this->value;
      }
    private:
      int32_t value;
  };
}
