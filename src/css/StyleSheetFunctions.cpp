#include <radiant/css/StyleSheetFunctions.h>

namespace Radiant::StyleSheetParser::Functions {
	Color rgb(Integer red, Integer green, Integer blue, Integer alpha) {
		return Color(red, green, blue, alpha);
	}
} // namespace Radiant::StyleSheetParser::Functions
