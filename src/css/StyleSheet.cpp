#include "radiant/css/StyleSheet.h"

#include "radiant/css/StyleSheetEntry.h"

#include <cstddef>

namespace Radiant::StyleSheetParser {
	void StyleSheet::add(StyleSheetProperty property, StyleSheetEntry entry) {
		this->styles[property] = entry;
	}

	size_t StyleSheet::size() const {
		return this->styles.size();
	}

	StyleSheetEntry StyleSheet::getOrDefault(StyleSheetProperty property, StyleSheetEntry defaultEntry) {
		if (this->styles.find(property) == this->styles.end()) {
			return defaultEntry;
		}
		return this->styles[property];
	}
} // namespace Radiant::StyleSheetParser
