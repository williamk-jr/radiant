#pragma once
#include "radiant/css/Token.h"

#include <list>
#include <memory>
#include <string>
#include <vector>

namespace Radiant::StyleSheetParser {
	enum class AstNodeType { ROOT, IDENTIFIER, FUNCTION, SELECTOR, PROPERTY, STRING, INTEGER, FLOAT, UNIT, COLOR };

	struct AstNode {
			AstNodeType type = AstNodeType::ROOT;
			Token token;
			AstNode* parent;
			std::vector<AstNode*> children;

			AstNode(AstNodeType type, Token token, AstNode* parent) : token(token), type(type), parent(parent) {}

			AstNode(Token token) : token(token) {}
	};
} // namespace Radiant::StyleSheetParser
