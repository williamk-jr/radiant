#include "radiant/css/StyleSheetEntry.h"

namespace Radiant::StyleSheetParser {
  StyleSheetEntry::StyleSheetEntry(size_t capacity) {
    this->values.reserve(capacity);
  }

  StyleSheetEntry::StyleSheetEntry(std::initializer_list<StyleSheetValue> values) {
    this->values.reserve(values.size());

    for (StyleSheetValue value : values) {
      this->values.push_back(value);
    }
  }

  void StyleSheetEntry::add(StyleSheetValue value) {
    this->values.emplace_back(value);
  }

  size_t StyleSheetEntry::size() {
    return this->values.size();
  }
}
