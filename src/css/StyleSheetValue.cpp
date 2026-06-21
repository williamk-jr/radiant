#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/values/Color.h"
#include "radiant/css/values/Unit.h"
#include <charconv>
#include <cstdint>
#include <variant>

namespace Radiant::StyleSheetParser {
  StyleSheetValue::StyleSheetValue(StyleSheetVariant value) : value(value) {}

  StyleSheetValue StyleSheetValue::fromString(ValueTypes type, std::string strValue) {
    switch (type) {
      case ValueTypes::UNIT: {
        return {Unit::fromString(strValue)};
      }
      case ValueTypes::COLOR: {
        return {Color::fromString(strValue)};
      }
      case ValueTypes::INTEGER: {
        int32_t integer = 0;
        std::from_chars_result result = std::from_chars(strValue.data(), strValue.data()+strValue.size(), integer);
        return {Integer(integer)};
      }
      case ValueTypes::FLOAT: {
        float floatingPoint = 0.0;
        std::from_chars_result result = std::from_chars(strValue.data(), strValue.data()+strValue.size(), floatingPoint);
        return {Float(floatingPoint)};
      }
      case ValueTypes::STRING: {
        return {strValue};
      }
    }
  }
}
