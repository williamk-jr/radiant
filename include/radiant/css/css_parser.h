
#pragma once
#include <filesystem>
#include <iostream>
#include "radiant/util/string_util.h"
#include "radiant/css/token.h"

class CssParser {
  public:
    std::vector<Token> tokenize(std::filesystem::path file);
    static std::string tokenTypeToString(TokenType tokenType);
  private:
    TokenType identifyToken(std::string token);
    void addToken(std::vector<Token>& tokenList, std::string token);
    
    bool isString(const std::string& token);
    bool isUnit(const std::string& token);
    bool isIdentifier(const std::string& token);
};
