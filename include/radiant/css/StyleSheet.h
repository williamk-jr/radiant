#pragma once

#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetProperty.h"
#include "radiant/css/StyleSheetValue.h"

#include <string>
#include <vector>

namespace Radiant::StyleSheetParser {
	class StyleSheet {
		public:
			void   add(StyleSheetProperty property, StyleSheetEntry entry);
			size_t size() const;

			StyleSheetEntry getOrDefault(StyleSheetProperty property, StyleSheetEntry defaultEntry);

		private:
			std::unordered_map<StyleSheetProperty, StyleSheetEntry> styles;
	};
} // namespace Radiant::StyleSheetParser
