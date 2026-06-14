#include "radiant/css/Token.h"

namespace Radiant {
  Token::Token(TokenType type, std::string value) : type(type), value(value) {}

  TokenType Token::getType() {
    return this->type;
  }
  std::string Token::getValue() {
    return this->value;
  }
}
