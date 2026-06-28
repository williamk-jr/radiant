#include "radiant/css/StyleSheet.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/Parser.h"
#include <cstddef>

namespace Radiant::StyleSheetParser {
  void StyleSheet::add(std::string name, StyleSheetEntry entry) {
    this->styles[name] = entry;
  }

  size_t StyleSheet::size() const {
    return this->styles.size();
  }

  StyleSheetEntry StyleSheet::getOrDefault(std::string name, StyleSheetEntry defaultEntry) {
    if (this->styles.find(name) == this->styles.end()) {
      return defaultEntry;
    }
    return this->styles[name];
  }
  
  StyleSheetEntry StyleSheet::getAbsolute(Parser& parser, std::string name) {
    PropertyEntry propertyEntry = parser.getPropertyEntry(name);
    return propertyEntry.resolver(
      this->getOrDefault(name, propertyEntry.defaultValue),
      0.0f
    );
  }
}
