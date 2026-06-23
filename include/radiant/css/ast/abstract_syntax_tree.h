#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <iostream>
#include <filesystem>

#include "radiant/css/StyleSheet.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/ast/ast_node.h"
#include "radiant/css/Token.h"

namespace Radiant::StyleSheetParser {
  struct AstLocation {
    AstNode* parent = nullptr;
    int index = 0;
  };

  class AbstractSyntaxTree : public AstNode {
    public:
      AbstractSyntaxTree(std::vector<Token> tokens);
      
      std::unordered_map<std::string, StyleSheet> toStyleSheets();
      void display();

    private:
      StyleSheetValue valueOfNode(AstNode* node);
  };
}
