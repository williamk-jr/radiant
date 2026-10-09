#include "radiant/css/Parser.h"

#include "radiant/css/Token.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>
#include <string>

TEST_CASE("Test CSS parser.", "[CSS_Parser]") {
	Radiant::StyleSheetParser::Parser parser{};

	SECTION("Basic identifier-block.") {
		std::string css = "identifier {}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Integer property.") {
		std::string css = "identifier {property: 20;}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},
		    {Radiant::StyleSheetParser::TokenType::INTEGER, "20"},
		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Float property.") {
		std::string css = "identifier {property: 20.0;}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},
		    {Radiant::StyleSheetParser::TokenType::FLOAT, "20.0"},
		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Integer Unit property.") {
		std::string css = "identifier {property: 20ft;}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},
		    {Radiant::StyleSheetParser::TokenType::UNIT, "20ft"},
		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Float Unit property.") {
		std::string css = "identifier {property: 20.0ft;}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},
		    {Radiant::StyleSheetParser::TokenType::UNIT, "20.0ft"},
		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Color property.") {
		std::string css = "identifier {property: #FFFFFFFF;}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},
		    {Radiant::StyleSheetParser::TokenType::COLOR, "#FFFFFFFF"},
		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Color Identifier property.") {
		std::string css = "identifier {property: red;}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},
		    {Radiant::StyleSheetParser::TokenType::COLOR, "red"},
		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Identifier property.") {
		std::string css = "identifier {property: property-identifier;}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property-identifier"},
		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}

	SECTION("Function property.") {
		std::string css = "identifier {property: function(param1, param2, param3);}";

		std::vector<Radiant::StyleSheetParser::Token> expectedTokens = {
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "identifier"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_OPEN, "{"},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "property"},
		    {Radiant::StyleSheetParser::TokenType::COLON, ":"},

		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "function"},
		    {Radiant::StyleSheetParser::TokenType::PARAMETER_LIST_OPEN, "("},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "param1"},
		    {Radiant::StyleSheetParser::TokenType::PARAMETER_LIST_SEPARATOR, ","},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "param2"},
		    {Radiant::StyleSheetParser::TokenType::PARAMETER_LIST_SEPARATOR, ","},
		    {Radiant::StyleSheetParser::TokenType::IDENTFIER, "param3"},
		    {Radiant::StyleSheetParser::TokenType::PARAMETER_LIST_CLOSE, ")"},

		    {Radiant::StyleSheetParser::TokenType::SEMI_COLON, ";"},
		    {Radiant::StyleSheetParser::TokenType::BLOCK_CLOSE, "}"}};

		REQUIRE(parser.tokenize(css) == expectedTokens);
	}
}
