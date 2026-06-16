#include "radiant/css/StyleSheet.h"

namespace Radiant {
  void StyleSheet::add(std::string name, StyleSheetEntry entry) {
    this->styles[name] = entry;
  }

  StyleSheetEntry StyleSheet::getOrDefault(std::string name, StyleSheetEntry defaultEntry) {
    if (this->styles.find(name) == this->styles.end()) {
      return defaultEntry;
    }
    return this->styles[name];
  }
}
