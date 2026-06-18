#include "radiant/css/ast/abstract_syntax_tree.h"
#include "radiant/css/ast/ast_node.h"
#include "radiant/util/logger/Logger.h"
#include <stdexcept>

namespace Radiant {
  AbstractSyntaxTree::AbstractSyntaxTree(std::vector<Token> tokens) : AstNode(Token(TokenType::IDENTFIER, "")) {
    AstNode* parent = this;
    for (int i = 0; i < tokens.size(); i++) {
      Token* token = &tokens.at(i);
      Token* lookahead = i+1 < tokens.size() ? &tokens.at(i+1) : nullptr;

      switch (token->getType()) {
        case TokenType::BLOCK_OPEN:
          parent = parent->children.at(parent->children.size()-1);
          break;
        case TokenType::COLON:
          parent = parent->children.at(parent->children.size()-1);
          break;
        case TokenType::BLOCK_CLOSE:
          parent = parent->parent;
          break;
        case TokenType::SEMI_COLON:
          parent = parent->parent;
          break;
        case TokenType::IDENTFIER:
          if (lookahead == nullptr) {
            break;
          }

          switch (lookahead->getType()) {
            case TokenType::BLOCK_OPEN:
              parent->children.push_back(new AstNode{AstNodeType::SELECTOR, *token, parent});
              break;
            case TokenType::COLON:
              parent->children.push_back(new AstNode{AstNodeType::PROPERTY, *token, parent});
              break;
            default:
              parent->children.push_back(new AstNode{AstNodeType::IDENTIFIER, *token, parent});
              break;
          }
          break;
        case TokenType::INTEGER:
          parent->children.push_back(new AstNode{AstNodeType::INTEGER, *token, parent});
          break;
        case TokenType::FLOAT:
          parent->children.push_back(new AstNode{AstNodeType::FLOAT, *token, parent});
          break;
        case TokenType::STRING:
          parent->children.push_back(new AstNode{AstNodeType::STRING, *token, parent});
          break;
        case TokenType::UNIT:
          parent->children.push_back(new AstNode{AstNodeType::UNIT, *token, parent});
          break;
        case TokenType::COLOR:
          parent->children.push_back(new AstNode{AstNodeType::COLOR, *token, parent});
          break;
        default:
          Logger::error("Unidentified token type.");
      }
    }
  }

  void AbstractSyntaxTree::display() {
    std::stack<std::pair<AstNode*, int>> nodes;
    nodes.push({this, -1});

    while (!nodes.empty()) {
      std::pair<AstNode*, int> entry = nodes.top();
      nodes.pop();
      
      std::string tabs;
      for (int i = 0; i < entry.second; i++) {
        tabs += "  ";
      }
      std::cout << tabs << entry.first->token.getValue() << "\n";

      for (AstNode* child : entry.first->children) {
        nodes.push({child, entry.second+1});
      } 
    }
  }
}
