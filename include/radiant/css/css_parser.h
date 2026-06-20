
#pragma once
#include <cstddef>
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
  using RuntimeFunction = std::function<StyleSheetValue(std::vector<StyleSheetValue>)>;

  struct PropertyEntry {
    std::vector<StyleSheetValueTypes> signature;
    StyleSheetEntry defaultValue;
    PropertyResolver resolver;
  };

  struct RegisteredFunction {
    RuntimeFunction runtimeFunction;
  };

  template<typename R, typename... Args, std::size_t... Is>
  static R invokeHelper(R(*fn)(Args...), std::vector<StyleSheetValue> params, std::index_sequence<Is...>) {
    return fn(((params[Is].get<MapValue<Args>::value>().value()), ...));
  }

  template<typename R, typename... Args>
  static RuntimeFunction wrapper(R(*fn)(Args...)) {
    return [fn](std::vector<StyleSheetValue> params) {
      return StyleSheetValue{ invokeHelper(fn, params, std::index_sequence_for<Args...>{}) }; 
    };
  };

  class CssParser {
    public:
      void registerProperty(std::string name, std::vector<StyleSheetValueTypes> signature, StyleSheetEntry defaultValue, PropertyResolver resolver);
      PropertyEntry getPropertyEntry(std::string name);


      template<typename Fn>
      RegisteredFunction registerFunction(std::string name, Fn function) {
        RuntimeFunction wrapper = Radiant::wrapper(function);
        return {wrapper};
      }

      std::unordered_map<std::string, StyleSheet> getStyleSheets(std::filesystem::path path);

      std::vector<Token> tokenize(std::filesystem::path file);
      static std::string tokenTypeToString(TokenType tokenType);
    private:
      std::unordered_map<std::string, PropertyEntry> propertyRegistry;
      std::unordered_map<std::string, RegisteredFunction> functionRegistry;

      TokenType identifyToken(std::string token);
      void addToken(std::vector<Token>& tokenList, std::string token);
      
      bool isString(const std::string& token);
      bool isUnit(const std::string& token);
      bool isIdentifier(const std::string& token);
  };
}
