
#pragma once
#include "radiant/css/StyleSheet.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/Token.h"
#include "radiant/css/ast/ast_node.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace Radiant::StyleSheetParser {

	struct PropertyEntry {
			std::vector<ValueTypes> signature;
			StyleSheetEntry         defaultValue;
	};

	class Parser {
		public:
			std::unordered_map<std::string, StyleSheet> getStyleSheets(std::filesystem::path path);

			std::vector<Token> tokenize(std::filesystem::path file);
			std::vector<Token> tokenize(std::string contents);
			static std::string tokenTypeToString(TokenType tokenType);

		private:
			TokenType       identifyToken(std::string token);
			StyleSheetValue getValue(AstNode* value);
			void            addToken(std::vector<Token>& tokenList, std::string token);

			bool isString(const std::string& token);
			bool isUnit(const std::string& token);
			bool isIdentifier(const std::string& token);
	};
} // namespace Radiant::StyleSheetParser
