
#include "radiant/css/Parser.h"
#include "radiant/css/Token.h"
#include "radiant/css/ast/abstract_syntax_tree.h"
#include "radiant/css/ast/ast_node.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>
#include <string>
#include <vector>

using namespace Radiant::StyleSheetParser;

// clang-format off
TEST_CASE("Test Stylesheet AST.", "[stylesheet_ast]") {
	Radiant::StyleSheetParser::Parser parser{};

	SECTION("Basic identifier-block.") {
		std::string css = "identifier {}";
    AbstractSyntaxTree tree{parser.tokenize(css)};

    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, {}}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Integer property.") {
		std::string css = "identifier {property: 20;}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::INTEGER, {TokenType::INTEGER, "20"}, std::vector<AstNode*>{}}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Float property.") {
		std::string css = "identifier {property: 20.0;}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::FLOAT, {TokenType::FLOAT, "20.0"}, std::vector<AstNode*>{}}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Integer Unit property.") {
		std::string css = "identifier {property: 20ft;}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::UNIT, {TokenType::UNIT, "20ft"}, std::vector<AstNode*>{}}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Float Unit property.") {
		std::string css = "identifier {property: 20.0ft;}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::UNIT, {TokenType::UNIT, "20.0ft"}, std::vector<AstNode*>{}}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Color property.") {
		std::string css = "identifier {property: #FFFFFFFF;}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::COLOR, {TokenType::COLOR, "#FFFFFFFF"}, std::vector<AstNode*>{}}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Color Identifier property.") {
		std::string css = "identifier {property: red;}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::COLOR, {TokenType::COLOR, "red"}, std::vector<AstNode*>{}}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Identifier property.") {
		std::string css = "identifier {property: property-identifier;}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "property-identifier"}, std::vector<AstNode*>{}}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Function property.") {
		std::string css = "identifier {property: function(param1, param2, param3);}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::FUNCTION, {TokenType::IDENTFIER, "function"}, std::vector<AstNode*>{

            new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "param1"}, std::vector<AstNode*>{}},
            new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "param2"}, std::vector<AstNode*>{}},
            new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "param3"}, std::vector<AstNode*>{}}
          }}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}

	SECTION("Nested function property.") {
		std::string css = "identifier {property: function(param1, param2, function2(param1, param2));}";
    Radiant::StyleSheetParser::AbstractSyntaxTree tree{parser.tokenize(css)};
    AbstractSyntaxTree expectedTree{std::vector<AstNode*>{
      new AstNode{AstNodeType::SELECTOR, {TokenType::IDENTFIER, "identifier"}, std::vector<AstNode*>{
        new AstNode{AstNodeType::PROPERTY, {TokenType::IDENTFIER, "property"}, std::vector<AstNode*>{
          new AstNode{AstNodeType::FUNCTION, {TokenType::IDENTFIER, "function"}, std::vector<AstNode*>{
            new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "param1"}, std::vector<AstNode*>{}},
            new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "param2"}, std::vector<AstNode*>{}},
            new AstNode{AstNodeType::FUNCTION, {TokenType::IDENTFIER, "function2"}, std::vector<AstNode*>{
              new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "param1"}, std::vector<AstNode*>{}},
              new AstNode{AstNodeType::IDENTIFIER, {TokenType::IDENTFIER, "param2"}, std::vector<AstNode*>{}},
            }}
          }}
        }}
      }}
    }};

		REQUIRE(tree == expectedTree);
	}
}

// clang-format on
