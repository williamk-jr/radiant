#include "radiant/css/Token.h"

namespace Radiant::StyleSheetParser {
	Token::Token(TokenType type, std::string value) : type(type), value(value) {}

	TokenType Token::getType() const {
		return this->type;
	}

	std::string Token::getValue() const {
		return this->value;
	}

	std::ostream& operator<<(std::ostream& os, const Token& obj) {
		return os << "\"" + obj.getValue() + "\"";
	}
} // namespace Radiant::StyleSheetParser
