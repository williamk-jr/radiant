#pragma once

#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetValue.h"
#include <string>
#include <vector>
namespace Radiant {
  class CssParser;

  class StyleSheet {
    public:
      void add(std::string name, StyleSheetEntry entry);
      StyleSheetEntry getOrDefault(std::string name, StyleSheetEntry defaultEntry);
      StyleSheetEntry getAbsolute(CssParser& parser, std::string name);
      
    private:
      std::unordered_map<std::string, StyleSheetEntry> styles;
  };
} 
