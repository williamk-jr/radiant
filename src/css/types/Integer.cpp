#include "radiant/css/types/Integer.h"

namespace Radiant::StyleSheetParser::Types {
	Integer::Integer() : value(0) {}

	Integer::Integer(int32_t value) : value(value) {}
} // namespace Radiant::StyleSheetParser::Types
