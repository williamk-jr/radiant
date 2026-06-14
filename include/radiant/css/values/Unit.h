#pragma once
#include <string>

namespace Radiant {
  enum class UnitType {
    PIXEL,
    PERCENTAGE
  };

  class Unit {
    public:
      Unit(float value, UnitType unit);

      float getValue();
      UnitType getUnit();

      static Unit fromString(std::string str);
    private:
      float value;
      UnitType unit;
  };
}
