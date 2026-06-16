#include "radiant/css/StyleSheetValue.h"
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
      case StyleSheetValueTypes::INTEGER: {
        uint32_t integer = 0;
        std::from_chars_result result = std::from_chars(strValue.data(), strValue.data()+strValue.size(), integer);
        return {integer};
      }
      case StyleSheetValueTypes::FLOAT: {
        float floatingPoint = 0.0;
        std::from_chars_result result = std::from_chars(strValue.data(), strValue.data()+strValue.size(), floatingPoint);
        return {floatingPoint};
      }
      case StyleSheetValueTypes::STRING: {
        return {strValue};
      }
    }
  }
}
