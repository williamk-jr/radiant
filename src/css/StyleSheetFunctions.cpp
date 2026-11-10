#include <radiant/css/StyleSheetFunctions.h>

namespace Radiant::StyleSheetParser::Functions {
	Types::Color rgb(Types::Integer red, Types::Integer green, Types::Integer blue, Types::Integer alpha) {
		return Types::Color(red, green, blue, alpha);
	}
} // namespace Radiant::StyleSheetParser::Functions
