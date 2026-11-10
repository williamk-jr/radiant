#include "radiant/css/StyleSheetValue.h"

#include "radiant/css/types/Color.h"
#include "radiant/css/types/Unit.h"

#include <charconv>
#include <cstdint>

namespace Radiant::StyleSheetParser {
	StyleSheetValue::StyleSheetValue(StyleSheetVariant value) : value(value) {}

	StyleSheetValue StyleSheetValue::fromString(ValueTypes type, std::string strValue) {
		switch (type) {
			case ValueTypes::UNIT: {
				return {Types::Unit::fromString(strValue)};
			}
			case ValueTypes::COLOR: {
				return {Types::Color::fromString(strValue)};
			}
			case ValueTypes::INTEGER: {
				int32_t                integer = 0;
				std::from_chars_result result =
				    std::from_chars(strValue.data(), strValue.data() + strValue.size(), integer);
				return {Types::Integer(integer)};
			}
			case ValueTypes::FLOAT: {
				float                  floatingPoint = 0.0;
				std::from_chars_result result =
				    std::from_chars(strValue.data(), strValue.data() + strValue.size(), floatingPoint);
				return {Types::Float(floatingPoint)};
			}
			case ValueTypes::STRING: {
				return {strValue};
			}
		}
	}
} // namespace Radiant::StyleSheetParser
