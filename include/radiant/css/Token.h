#pragma once
#include <string>

namespace Radiant::StyleSheetParser {
  enum class TokenType {
    BLOCK_OPEN,
    BLOCK_CLOSE,
    PARAMETER_LIST_OPEN,
    PARAMETER_LIST_CLOSE,
    PARAMETER_LIST_SEPARATOR,
    COLON,
    SEMI_COLON,
    IDENTFIER,
    STRING,
    INTEGER,
    FLOAT,
    UNIT,
    COLOR,
    INVALID
  };

  class Token {
    public:
      Token(TokenType type, std::string value);

      TokenType getType();
      std::string getValue();

    private:
      TokenType type;
      std::string value;
  };
}

