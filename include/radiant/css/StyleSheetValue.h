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

      template<typename T>
      std::optional<T> get() {
        const T* value = std::get_if<T>(&this->value);
        if (value == nullptr) {
          return std::nullopt;
        }
        return *value;
      }


      static StyleSheetValue fromString(StyleSheetValueTypes type, std::string strValue);

    private:
      StyleSheetVariant value;
  };
} 
