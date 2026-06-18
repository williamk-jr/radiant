#pragma once 

#include "radiant/css/values/Unit.h"
#include <cstdint>
#include <optional>
#include <string>
#include <variant>

namespace Radiant {
  enum class StyleSheetValueTypes {
    INTEGER,
    FLOAT,
    STRING,
    UNIT
  };

  typedef std::variant<
    std::string, 
    uint32_t, 
    float,
    Unit
  > StyleSheetVariant;

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
          float* unit = std::get_if<float>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<float>{*unit};
        } else if constexpr (T == StyleSheetValueTypes::INTEGER) {
          uint32_t* unit = std::get_if<uint32_t>(&this->value);
          return unit == nullptr ? std::nullopt : std::optional<uint32_t>{*unit};
        }
      }

      static StyleSheetValue fromString(StyleSheetValueTypes type, std::string strValue);

    private:
      StyleSheetVariant value;
  };
} 
