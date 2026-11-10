#pragma once

#include "radiant/css/types/Color.h"
#include "radiant/css/types/Float.h"
#include "radiant/css/types/Integer.h"

namespace Radiant::StyleSheetStandardFunctions {
	StyleSheetParser::Types::Color rgb(StyleSheetParser::Types::Integer red,
	                                   StyleSheetParser::Types::Integer green,
	                                   StyleSheetParser::Types::Integer blue,
	                                   StyleSheetParser::Types::Integer alpha);
}
