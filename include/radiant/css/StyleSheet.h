#pragma once

#include "radiant/css/StyleSheetValue.h"
#include <string>
#include <vector>
namespace Radiant {
  typedef std::vector<StyleSheetValue> StyleSheetEntry;

  class StyleSheet {
    public:
      void add(std::string name, StyleSheetEntry entry);
      StyleSheetEntry getOrDefault(std::string name, StyleSheetEntry defaultEntry);
      
    private:
      std::unordered_map<std::string, StyleSheetEntry> styles;
  };
} 
