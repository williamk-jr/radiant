#pragma once

#include "radiant/css/types/Color.h"
#include "radiant/css/types/Float.h"
#include "radiant/css/types/Function.h"
#include "radiant/css/types/Integer.h"
#include "radiant/css/types/StyleSheetTypes.h"
#include "radiant/css/types/Unit.h"

#include <charconv>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

namespace Radiant::StyleSheetParser {
	typedef std::variant<std::string, Types::Function, Types::Integer, Types::Float, Types::Unit, Types::Color>
	    StyleSheetVariant;

	class StyleSheetValue {
		public:
			StyleSheetValue(StyleSheetVariant value);

			template <Types::StyleSheetType T>
			auto get() {
				T* unit = std::get_if<T>(&value);
				return unit == nullptr ? std::nullopt : std::optional<T>{*unit};
			}

			template <Types::StyleSheetType T>
			bool isType() {
				return std::holds_alternative<T>(this->value);
			}

			template <Types::StyleSheetType T>
			static StyleSheetValue fromString(std::string strValue) {
				if constexpr (std::is_same_v<T, Types::Unit>) {
					return {Types::Unit::fromString(strValue)};
				} else if constexpr (std::is_same_v<T, Types::Color>) {
					return {Types::Color::fromString(strValue)};
				} else if constexpr (std::is_same_v<T, Types::Integer>) {
					int32_t                integer = 0;
					std::from_chars_result result =
					    std::from_chars(strValue.data(), strValue.data() + strValue.size(), integer);
					return {Types::Integer(integer)};
				} else if constexpr (std::is_same_v<T, Types::Float>) {
					float                  floatingPoint = 0.0;
					std::from_chars_result result =
					    std::from_chars(strValue.data(), strValue.data() + strValue.size(), floatingPoint);
					return {Types::Float(floatingPoint)};
				} else if constexpr (std::is_same_v<T, std::string>) {
					return {strValue};
				}
			}

		private:
			StyleSheetVariant value;
	};
} // namespace Radiant::StyleSheetParser
