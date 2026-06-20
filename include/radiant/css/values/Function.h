#pragma once

#include "radiant/css/StyleSheetValue.h"
#include <string>
#include <vector>
namespace Radiant {
  class Function {
    public:
      Function(std::string name, std::vector<StyleSheetValue> parameters);
    private:
      std::string name;
      std::vector<StyleSheetValue> parameters;
  };
}
