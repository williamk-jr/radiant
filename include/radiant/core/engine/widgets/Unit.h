#pragma once

#include <cstdint>
namespace Radiant {
  enum class UnitType {
    PIXELS,
    PERCENTAGE,
    VIEWPORT_WIDTH,
    VIEWPORT_HEIGHT
  };

  class Unit {
    public:
      Unit();
      Unit(UnitType type, uint32_t value);

    private:
      UnitType type;
      uint32_t value = 0;
  };
}
