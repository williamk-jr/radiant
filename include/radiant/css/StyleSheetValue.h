#pragma once 

#include "radiant/css/values/Color.h"
#include "radiant/css/values/Unit.h"
#include "radiant/css/values/Integer.h"
#include "radiant/css/values/Float.h"
#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace Radiant {
  enum class StyleSheetValueTypes {
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
    LibStyleSheets::Color
  > StyleSheetVariant;

  template<typename T>
  struct MapValue;

  template<>
  struct MapValue<std::string> {
    static constexpr StyleSheetValueTypes value = StyleSheetValueTypes::STRING;
  };
  template<>
  struct MapValue<Integer> {
    static constexpr StyleSheetValueTypes value = StyleSheetValueTypes::INTEGER;
  };
  template<>
  struct MapValue<Float> {
    static constexpr StyleSheetValueTypes value = StyleSheetValueTypes::FLOAT;
  };
  template<>
  struct MapValue<Unit> {
    static constexpr StyleSheetValueTypes value = StyleSheetValueTypes::UNIT;
  };
  template<>
  struct MapValue<LibStyleSheets::Color> {
    static constexpr StyleSheetValueTypes value = StyleSheetValueTypes::COLOR;
  };

  class StyleSheetValue {
    public:
      StyleSheetValue(StyleSheetVariant value);

      template<StyleSheetValueTypes T>
      auto get() {
        if constexpr (T == StyleSheetValueTypes::UNIT) {
          Unit* unit = std::get_if<Unit>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<Unit>{*unit};
        } else if constexpr (T == StyleSheetValueTypes::STRING) {
          std::string* unit = std::get_if<std::string>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<std::string>{*unit};
        } else if constexpr (T == StyleSheetValueTypes::FLOAT) {
          Float* unit = std::get_if<Float>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<Float>{*unit};
        } else if constexpr (T == StyleSheetValueTypes::INTEGER) {
          Integer* unit = std::get_if<Integer>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<Integer>{*unit};
        } else if constexpr (T == StyleSheetValueTypes::COLOR) {
          LibStyleSheets::Color* unit = std::get_if<LibStyleSheets::Color>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<LibStyleSheets::Color>{*unit};
        }
      }

      static StyleSheetValue fromString(StyleSheetValueTypes type, std::string strValue);

    private:
      StyleSheetVariant value;
  };
} 
