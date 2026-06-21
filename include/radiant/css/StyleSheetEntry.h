#pragma once

#include "radiant/css/StyleSheetValue.h"
#include <cstddef>
#include <initializer_list>
#include <vector>
namespace Radiant::StyleSheetParser {
  class StyleSheetEntry {
    public:
      StyleSheetEntry() = default;
      StyleSheetEntry(size_t capacity);
      StyleSheetEntry(std::initializer_list<StyleSheetValue> values);

      void add(StyleSheetValue value);

      template<ValueTypes T>
      auto get(size_t index) {
        return this->values[index].get<T>();
      }

      size_t size();
    private:
      std::vector<StyleSheetValue> values;
  };
}
