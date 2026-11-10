#pragma once
#include <radiant/css/types/Color.h>
#include <radiant/css/types/Integer.h>

namespace Radiant::StyleSheetParser::Functions {
	Types::Color rgb(Types::Integer red, Types::Integer green, Types::Integer blue, Types::Integer alpha);
} // namespace Radiant::StyleSheetParser::Functions
