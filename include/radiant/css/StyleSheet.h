#pragma once

#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetValue.h"
#include <string>
#include <vector>
namespace Radiant::StyleSheetParser {
  class Parser;

  class StyleSheet {
    public:
      void add(std::string name, StyleSheetEntry entry);
      StyleSheetEntry getOrDefault(std::string name, StyleSheetEntry defaultEntry);
      StyleSheetEntry getAbsolute(Parser& parser, std::string name);
      
    private:
      std::unordered_map<std::string, StyleSheetEntry> styles;
  };
} 
