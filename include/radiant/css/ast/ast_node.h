#pragma once
#include <memory>
#include <string>
#include <list>
#include <vector>
#include "radiant/css/Token.h"

namespace Radiant::StyleSheetParser {
  enum class AstNodeType {
    ROOT,
    IDENTIFIER,
    FUNCTION,
    SELECTOR,
    PROPERTY,
    STRING,
    INTEGER,
    FLOAT,
    UNIT,
    COLOR
  };

  struct AstNode {
    AstNodeType type = AstNodeType::ROOT;
    Token token;
    AstNode* parent;
    std::vector<AstNode*> children;

    AstNode(AstNodeType type, Token token, AstNode* parent) : token(token), type(type), parent(parent) {}
    AstNode(Token token) : token(token) {}
  };
}
