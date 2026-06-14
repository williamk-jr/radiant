#include "radiant/css/values/Unit.h"

namespace Radiant {
  Unit::Unit(float value, UnitType unit) : value(value), unit(unit) {}


  float Unit::getValue() {
    return this->value;
  }

  UnitType Unit::getUnit() {
    return this->unit;
  }

  Unit Unit::fromString(std::string str) {
     
  }
}
