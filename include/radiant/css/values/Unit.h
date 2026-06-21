#pragma once
#include "radiant/css/values/IValue.h"
#include <string>
#include <unordered_map>

namespace Radiant::StyleSheetParser {
  enum class UnitType {
    PIXEL,
    PERCENTAGE
  };

  class Unit : IValue<Unit> {
    public:
      Unit(float value, UnitType unit);

      float getValue();
      UnitType getUnit();

      Unit resolve() override;

      static Unit fromString(std::string str);
    private:
      float value;
      UnitType unit;

      static std::unordered_map<std::string, UnitType> UNIT_MAP;
  };
}
