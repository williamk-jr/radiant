#pragma once
#include <vector>
#include <stack>
#include <iostream>

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

      void display();

    private:
  };
}
