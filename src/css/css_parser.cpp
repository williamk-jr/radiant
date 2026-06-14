#include "radiant/css/css_parser.h"
#include "radiant/css/ast/abstract_syntax_tree.h"
#include "radiant/css/ast/ast_node.h"
#include <cctype>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>


namespace Radiant {
  std::unordered_map<std::string, StyleSheet> CssParser::getStyleSheets(std::filesystem::path path) {
    std::vector<Token> tokens = this->tokenize(path); 
    AbstractSyntaxTree abstractSyntaxTree(tokens); 

    std::unordered_map<std::string, StyleSheet> stylesheets;
    
    for (AstNode* node : abstractSyntaxTree.children) {
      if (node->type == AstNodeType::PROPERTY) {
        StyleSheetValue styleSheetValue(node->children.size()); 

        for (AstNode* valueNode : node->children) {
          std::string tokenValue(valueNode->token.getValue());

          styleSheetValue.emplace_back();
        }
      }
    }
    return stylesheets;
  }

  std::vector<Token> CssParser::tokenize(std::filesystem::path file) {
    if (!std::filesystem::exists(file)) {
      std::cout << "Invalid Path: " << std::filesystem::absolute(file) << "\n";
    }

    std::ifstream stream(file);
    std::vector<Token> tokens;
   
    std::string line;
    std::string fileContents;
    while (std::getline(stream, line)) {
      fileContents += line;
    }
    
    
    std::string currentToken;
    bool isString = false;
    for (int i = 0; i < fileContents.size(); i++) {
      char currentChar = fileContents.at(i);

      // Ignore whitespace & newlines
      if ((std::isblank(currentChar) && currentChar != ' ') || currentChar == '\n') {
        continue;
      }

      if (isString) {
        currentToken += currentChar;
        if (currentChar == '"') {
          this->addToken(tokens, currentToken);
          currentToken = "";
          isString = false;
        }
        continue;
      }

      // Check delimiters
      switch (currentChar) {
        case '"':
          if (fileContents.at(i-1) != '\\') {
            this->addToken(tokens, currentToken);
            currentToken = "";
            currentToken += '"';
            isString = true;
          }
          break;
        case ' ':
          if (!currentToken.empty()) {
            this->addToken(tokens, currentToken);
            currentToken = "";
          }
          break;

        case '{':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::BLOCK_OPEN, "{"));
          currentToken = "";
          break;

        case '}':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::BLOCK_CLOSE, "}"));
          currentToken = "";
          break;

        case '(':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::BLOCK_CLOSE, "}"));
          currentToken = "";
          break;

        case ')':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::BLOCK_CLOSE, "}"));
          currentToken = "";
          break;

        case ':':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::COLON, ":"));
          currentToken = "";
          break;

        case ';':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::SEMI_COLON, ";"));
          currentToken = "";
          break;

        case '[':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::BLOCK_CLOSE, "}"));
          currentToken = "";
          break;

        case ']':
          this->addToken(tokens, currentToken);
          tokens.push_back(Token(TokenType::BLOCK_CLOSE, "}"));
          currentToken = "";
          break;

        default:
          currentToken += currentChar;
      }
    }

   // for (Token token : tokens) {
   //   std::cout << CssParser::tokenTypeToString(token.getType()) << ": " << token.getValue() << "\n";
   // }
   // std::cout << fileContents << "\n";
    stream.close();
    return tokens;
  };

  std::string CssParser::tokenTypeToString(TokenType tokenType) {
    switch (tokenType) {
      case TokenType::BLOCK_OPEN: return "BLOCK_OPEN";
      case TokenType::BLOCK_CLOSE: return "BLOCK_CLOSE";
      case TokenType::COLON: return "COLON";
      case TokenType::SEMI_COLON: return "SEMI_COLON";
      case TokenType::IDENTFIER: return "IDENTIFIER";
      case TokenType::STRING: return "STRING";
      case TokenType::INTEGER: return "INTEGER";
      case TokenType::FLOAT: return "FLOAT";
      case TokenType::UNIT: return "UNIT";
      case TokenType::INVALID: return "INVALID";
      default: throw std::invalid_argument("Unknown TokenType");
    }
  }

  TokenType CssParser::identifyToken(std::string token) {
    if (this->isString(token)) {
      return TokenType::STRING;
    } else if (this->isUnit(token)) {
      return TokenType::UNIT; 
    } else if (string_util::isFloat(token)) {
      return TokenType::FLOAT;
    } else if (string_util::isInteger(token)) {
      return TokenType::INTEGER;
    } else if (this->isIdentifier(token)) {
      return TokenType::IDENTFIER;
    }
    return TokenType::INVALID;
  }

  void CssParser::addToken(std::vector<Token>& tokens, std::string token) {
    if (token.empty()) {
      return;
    }

    tokens.push_back(Token(
      this->identifyToken(token), 
      token
    ));
  }

  bool CssParser::isString(const std::string& token) {
    return (string_util::startsWith(token, "\"") && string_util::endsWith(token, "\"")) ||
            string_util::startsWith(token, "\'") && string_util::endsWith(token, "\'");
  }

  bool CssParser::isUnit(const std::string& token) {
    int lastDigitIndex = string_util::findLastOf(token, string_util::isCharNumeric);
    if (lastDigitIndex == -1) {
      return false;
    }

    std::string number = token.substr(0, lastDigitIndex+1);
    std::string unit = token.substr(lastDigitIndex+1, token.size());

    return string_util::isNumeric(number) && string_util::isAlphabetic(unit);
  }

  bool CssParser::isIdentifier(const std::string& token) {
    return !string_util::isCharNumeric(token.at(0)) && string_util::containsOnly(token, [](unsigned char c) {
      return string_util::isCharAlphanumeric(c) || c == '-';
    }); 
  }
}

