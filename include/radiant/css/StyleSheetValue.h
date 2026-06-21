#pragma once 

#include "radiant/css/values/Color.h"
#include "radiant/css/values/Unit.h"
#include "radiant/css/values/Integer.h"
#include "radiant/css/values/Float.h"
#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace Radiant::StyleSheetParser {
  enum class ValueTypes {
    INTEGER,
    FLOAT,
    STRING,
    UNIT,
    COLOR,
    FUNCTION
  };

  typedef std::variant<
    std::string, 
    Integer, 
    Float,
    Unit,
    Color
  > StyleSheetVariant;

  template<typename T>
  struct MapValue;

  template<>
  struct MapValue<std::string> {
    static constexpr ValueTypes value = ValueTypes::STRING;
  };
  template<>
  struct MapValue<Integer> {
    static constexpr ValueTypes value = ValueTypes::INTEGER;
  };
  template<>
  struct MapValue<Float> {
    static constexpr ValueTypes value = ValueTypes::FLOAT;
  };
  template<>
  struct MapValue<Unit> {
    static constexpr ValueTypes value = ValueTypes::UNIT;
  };
  template<>
  struct MapValue<Color> {
    static constexpr ValueTypes value = ValueTypes::COLOR;
  };

  class StyleSheetValue {
    public:
      StyleSheetValue(StyleSheetVariant value);

      template<ValueTypes T>
      auto get() {
        if constexpr (T == ValueTypes::UNIT) {
          Unit* unit = std::get_if<Unit>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<Unit>{*unit};
        } else if constexpr (T == ValueTypes::STRING) {
          std::string* unit = std::get_if<std::string>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<std::string>{*unit};
        } else if constexpr (T == ValueTypes::FLOAT) {
          Float* unit = std::get_if<Float>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<Float>{*unit};
        } else if constexpr (T == ValueTypes::INTEGER) {
          Integer* unit = std::get_if<Integer>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<Integer>{*unit};
        } else if constexpr (T == ValueTypes::COLOR) {
          Color* unit = std::get_if<Color>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<Color>{*unit};
        }
      }

      static StyleSheetValue fromString(ValueTypes type, std::string strValue);

    private:
      StyleSheetVariant value;
  };
} 
