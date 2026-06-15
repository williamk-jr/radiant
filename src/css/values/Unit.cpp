#include "radiant/css/values/Unit.h"
#include "radiant/util/logger/Logger.h"
#include "radiant/util/string_util.h"
#include <charconv>

namespace Radiant {
  std::unordered_map<std::string, UnitType> Unit::UNIT_MAP{
    {"%", UnitType::PERCENTAGE},
    {"px", UnitType::PIXEL}
  };

  Unit::Unit(float value, UnitType unit) : value(value), unit(unit) {}


  float Unit::getValue() {
    return this->value;
  }

  UnitType Unit::getUnit() {
    return this->unit;
  }

  Unit Unit::fromString(std::string str) {
    int valueEnd = string_util::findLastOf(str, [](unsigned char c) {
      return string_util::isCharNumeric(c);
    }); 

    float value = 0.0;
    std::from_chars(str.data(), str.data()+valueEnd+1, value);

    std::string stringUnit(str.substr(valueEnd+1, str.size()));
    auto unitIterator = Unit::UNIT_MAP.find(stringUnit);

    if (unitIterator == Unit::UNIT_MAP.end()) {
      Logger::error("Unit of type \""+stringUnit+"\" could not be found.");
    }

    return {value, unitIterator->second};

  }
}
