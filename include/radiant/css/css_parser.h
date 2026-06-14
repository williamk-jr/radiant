
#pragma once
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "radiant/core/engine/widgets/Unit.h"
#include "radiant/util/string_util.h"
#include "radiant/css/Token.h"

namespace Radiant {
  typedef std::vector<std::variant<
    std::string, 
    uint32_t, 
    Unit
  >> StyleSheetValue;

  typedef std::unordered_map<std::string, StyleSheetValue> StyleSheet;

  class CssParser {
    public:
      std::unordered_map<std::string, StyleSheet> getStyleSheets(std::filesystem::path path);

      std::vector<Token> tokenize(std::filesystem::path file);
      static std::string tokenTypeToString(TokenType tokenType);
    private:
      TokenType identifyToken(std::string token);
      void addToken(std::vector<Token>& tokenList, std::string token);
      
      bool isString(const std::string& token);
      bool isUnit(const std::string& token);
      bool isIdentifier(const std::string& token);
  };
}
