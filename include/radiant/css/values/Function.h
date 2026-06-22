#pragma once

#include <string>
#include <vector>
namespace Radiant::StyleSheetParser {
  class StyleSheetValue;

  class Function {
    public:
      Function(std::string name, std::vector<StyleSheetValue> parameters);
    private:
      std::string name;
      std::vector<StyleSheetValue> parameters;
  };
}
