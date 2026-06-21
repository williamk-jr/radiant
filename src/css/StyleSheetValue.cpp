#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/values/Color.h"
#include "radiant/css/values/Unit.h"
#include <charconv>
#include <cstdint>
#include <variant>

namespace Radiant {
  StyleSheetValue::StyleSheetValue(StyleSheetVariant value) : value(value) {}

  StyleSheetValue StyleSheetValue::fromString(StyleSheetValueTypes type, std::string strValue) {
    switch (type) {
      case StyleSheetValueTypes::UNIT: {
        return {Unit::fromString(strValue)};
      }
      case StyleSheetValueTypes::COLOR: {
        return {LibStyleSheets::Color::fromString(strValue)};
      }
      case StyleSheetValueTypes::INTEGER: {
        int32_t integer = 0;
        std::from_chars_result result = std::from_chars(strValue.data(), strValue.data()+strValue.size(), integer);
        return {Integer(integer)};
      }
      case StyleSheetValueTypes::FLOAT: {
        float floatingPoint = 0.0;
        std::from_chars_result result = std::from_chars(strValue.data(), strValue.data()+strValue.size(), floatingPoint);
        return {Float(floatingPoint)};
      }
      case StyleSheetValueTypes::STRING: {
        return {strValue};
      }
    }
  }
}
