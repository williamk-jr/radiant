#pragma once
#include <string>

namespace Radiant {
  enum class TokenType {
    BLOCK_OPEN,
    BLOCK_CLOSE,
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

