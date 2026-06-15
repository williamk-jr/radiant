#pragma once 

#include "radiant/css/values/Unit.h"
#include <cstdint>
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
      static StyleSheetValue fromString(StyleSheetValueTypes type, std::string strValue);

    private:
      StyleSheetVariant value;
  };
} 
