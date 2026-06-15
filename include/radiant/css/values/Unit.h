#pragma once
#include <string>
#include <unordered_map>

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

      static std::unordered_map<std::string, UnitType> UNIT_MAP;
  };
}
