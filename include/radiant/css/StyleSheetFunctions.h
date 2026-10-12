#pragma once
#include <radiant/css/values/Color.h>
#include <radiant/css/values/Integer.h>

namespace Radiant::StyleSheetParser::Functions {
	Color rgb(Integer red, Integer green, Integer blue, Integer alpha);
} // namespace Radiant::StyleSheetParser::Functions
