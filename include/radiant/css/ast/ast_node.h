#pragma once
#include "radiant/css/Token.h"

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace Radiant::StyleSheetParser {
	enum class AstNodeType { ROOT, IDENTIFIER, FUNCTION, SELECTOR, PROPERTY, STRING, INTEGER, FLOAT, UNIT, COLOR };

	struct AstNode {
			AstNodeType           type = AstNodeType::ROOT;
			Token                 token;
			AstNode*              parent;
			std::vector<AstNode*> children;

			AstNode(AstNodeType type, Token token, AstNode* parent) : token(token), type(type), parent(parent) {}

			AstNode(AstNodeType type, Token token, std::vector<AstNode*> children)
			    : token(token), type(type), children(children) {}

			AstNode(Token token) : token(token) {}

			bool operator==(const AstNode& other) const {
				bool sameType     = this->type == other.type;
				bool sameToken    = this->token == other.token;
				bool sameChildren = true;

				if (this->children.size() != other.children.size()) {
					sameChildren = false;
				} else {

					for (int i = 0; i < this->children.size(); i++) {
						if (*this->children[i] != *other.children[i]) {
							std::cout << "Non equal: " << this->children[i]->token.getValue() << " and "
							          << other.children[i]->token.getValue() << "\n";
							sameChildren = false;
						}
					}
				}
				return sameType && sameToken && sameChildren;
			}
	};
} // namespace Radiant::StyleSheetParser
