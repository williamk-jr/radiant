#pragma once
#include "radiant/css/types/Color.h"
#include "radiant/css/types/Float.h"
#include "radiant/css/types/Function.h"
#include "radiant/css/types/Integer.h"
#include "radiant/css/types/Unit.h"

#include <concepts>

namespace Radiant::StyleSheetParser::Types {
	template <typename T>
	concept StyleSheetType = std::is_same_v<T, Unit> || std::is_same_v<T, Color> || std::is_same_v<T, Integer>
	                      || std::is_same_v<T, Float> || std::is_same_v<T, Function> || std::is_same_v<T, std::string>;
}
