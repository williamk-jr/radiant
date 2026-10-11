#pragma once
#include "radiant/css/StyleSheet.h"
#include "radiant/css/StyleSheetProperty.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/Token.h"
#include "radiant/css/ast/ast_node.h"

#include <filesystem>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace Radiant::StyleSheetParser {
	struct AstLocation {
			AstNode* parent = nullptr;
			int      index  = 0;
	};

	class AbstractSyntaxTree : public AstNode {
		public:
			AbstractSyntaxTree(std::vector<Token> tokens);

			std::unordered_map<std::string, StyleSheet> toStyleSheets();
			void                                        display();

		private:
			StyleSheetValue valueOfNode(AstNode* node);

			inline static const std::unordered_map<std::string, StyleSheetProperty> PROPERTY_MAP{
			    {"width", StyleSheetProperty::WIDTH},
			    {"height", StyleSheetProperty::HEIGHT},
			    {"left", StyleSheetProperty::LEFT},
			    {"right", StyleSheetProperty::RIGHT},
			    {"top", StyleSheetProperty::TOP},
			    {"bottom", StyleSheetProperty::BOTTOM},
			    {"color", StyleSheetProperty::COLOR},
			    {"background-color", StyleSheetProperty::BACKGROUND_COLOR},
			    {"margin", StyleSheetProperty::MARGIN},
			    {"margin-left", StyleSheetProperty::MARGIN_LEFT},
			    {"margin-right", StyleSheetProperty::MARGIN_RIGHT},
			    {"margin-top", StyleSheetProperty::MARGIN_TOP},
			    {"margin-bottom", StyleSheetProperty::MARGIN_BOTTOM},
			};
	};
} // namespace Radiant::StyleSheetParser
