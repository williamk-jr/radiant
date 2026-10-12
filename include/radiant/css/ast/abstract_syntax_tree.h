#pragma once
#include "radiant/css/StyleSheet.h"
#include "radiant/css/StyleSheetFunctions.h"
#include "radiant/css/StyleSheetProperty.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/Token.h"
#include "radiant/css/ast/ast_node.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Radiant::StyleSheetParser {
	struct AstLocation {
			AstNode* parent = nullptr;
			int      index  = 0;
	};

	using RuntimeFunction = std::function<StyleSheetValue(std::vector<StyleSheetValue>)>;

	template <typename R, typename... Args, std::size_t... Is>
	static R
	invokeHelper(R (*fn)(Args...), std::vector<StyleSheetValue> params, std::index_sequence<Is...> indexSequence) {
		return fn(params[Is].get<MapTypeToEnum<Args>::value>().value()...);
	}

	template <typename R, typename... Args>
	static RuntimeFunction registerFunction(R (*fn)(Args...)) {
		return [fn](std::vector<StyleSheetValue> params) {
			return StyleSheetValue{invokeHelper(fn, params, std::index_sequence_for<Args...>{})};
		};
	};

	class AbstractSyntaxTree : public AstNode {
		public:
			AbstractSyntaxTree(std::vector<Token> tokens);

			AbstractSyntaxTree(std::vector<AstNode*> children);

			// bool operator==(const AbstractSyntaxTree& other) const;

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

			inline static const std::unordered_map<std::string, RuntimeFunction> FUNCTION_MAP{
			    {"rgb", registerFunction(&Functions::rgb)}};
	};

	std::ostream& operator<<(std::ostream& os, const AbstractSyntaxTree& obj);
} // namespace Radiant::StyleSheetParser
