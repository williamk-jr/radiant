#include "radiant/core/engine/StyleSheetStandardFunctions.h"

#include "radiant/css/types/Color.h"

namespace Radiant::StyleSheetStandardFunctions {

	StyleSheetParser::Types::Color rgb(StyleSheetParser::Types::Integer red,
	                                   StyleSheetParser::Types::Integer green,
	                                   StyleSheetParser::Types::Integer blue,
	                                   StyleSheetParser::Types::Integer alpha) {
		return StyleSheetParser::Types::Color(red, green, blue, alpha);
	}
} // namespace Radiant::StyleSheetStandardFunctions
