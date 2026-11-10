#include "radiant/core/engine/StyleSheetPropertyResolvers.h"

#include "radiant/css/types/Unit.h"
#include "radiant/util/logger/Logger.h"

#include <string>

namespace Radiant::PropertyResolvers {
	StyleSheetParser::StyleSheetEntry UNIT_LIST_RESOLVER(StyleSheetParser::StyleSheetEntry values, float context) {
		StyleSheetParser::StyleSheetEntry resolvedValues(values.size());
		// Logger::info("Size: " + std::to_string(values.size()));
		for (int i = 0; i < values.size(); i++) {
			StyleSheetParser::Types::Unit relativeValue = values.get<StyleSheetParser::Types::Unit>(i).value();
			// Logger::info("Value: " + std::to_string(relativeValue.getValue()));
			resolvedValues.add({relativeValue.resolve(0)});
		}

		return resolvedValues;
	}
} // namespace Radiant::PropertyResolvers
