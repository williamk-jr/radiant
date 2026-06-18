
#pragma once
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include "radiant/css/StyleSheet.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/util/string_util.h"
#include "radiant/css/Token.h"

namespace Radiant {
  using PropertyResolver = StyleSheetEntry (*)(StyleSheetEntry, float);

  struct PropertyEntry {
    std::vector<StyleSheetValueTypes> signature;
    StyleSheetEntry defaultValue;
    PropertyResolver resolver;
  };

  class CssParser {
    public:
      void registerProperty(std::string name, std::vector<StyleSheetValueTypes> signature, StyleSheetEntry defaultValue, PropertyResolver resolver);
      PropertyEntry getPropertyEntry(std::string name);

      std::unordered_map<std::string, StyleSheet> getStyleSheets(std::filesystem::path path);

      std::vector<Token> tokenize(std::filesystem::path file);
      static std::string tokenTypeToString(TokenType tokenType);
    private:
      std::unordered_map<std::string, PropertyEntry> propertyRegistry;

      TokenType identifyToken(std::string token);
      void addToken(std::vector<Token>& tokenList, std::string token);
      
      bool isString(const std::string& token);
      bool isUnit(const std::string& token);
      bool isIdentifier(const std::string& token);
  };
}
