#pragma once

#include "radiant/css/types/Color.h"
#include "radiant/css/types/Float.h"
#include "radiant/css/types/Function.h"
#include "radiant/css/types/Integer.h"
#include "radiant/css/types/StyleSheetTypes.h"
#include "radiant/css/types/Unit.h"

#include <optional>
#include <string>
#include <variant>

namespace Radiant::StyleSheetParser {
	enum class ValueTypes { INTEGER, FLOAT, STRING, UNIT, COLOR, FUNCTION };

	typedef std::variant<std::string, Types::Function, Types::Integer, Types::Float, Types::Unit, Types::Color>
	    StyleSheetVariant;

	template <typename T>
	struct MapTypeToEnum;

	template <ValueTypes T>
	struct MapEnumToType;

/* Macro to create type specializations for mapping between literal and enum types.*/
#define REGISTER_TYPE_MAP(type, enum_type)                                                                             \
	template <>                                                                                                        \
	struct MapTypeToEnum<type> {                                                                                       \
			static constexpr ValueTypes value = enum_type;                                                             \
	};                                                                                                                 \
                                                                                                                       \
	template <>                                                                                                        \
	struct MapEnumToType<enum_type> {                                                                                  \
			using value = type;                                                                                        \
	};

	REGISTER_TYPE_MAP(std::string, ValueTypes::STRING);
	REGISTER_TYPE_MAP(Types::Unit, ValueTypes::UNIT);
	REGISTER_TYPE_MAP(Types::Integer, ValueTypes::INTEGER);
	REGISTER_TYPE_MAP(Types::Float, ValueTypes::FLOAT);
	REGISTER_TYPE_MAP(Types::Color, ValueTypes::COLOR);
	REGISTER_TYPE_MAP(Types::Function, ValueTypes::FUNCTION);
#undef REGISTER_TYPE_MAP

	class StyleSheetValue {
		public:
			StyleSheetValue(StyleSheetVariant value);

			template <Types::StyleSheetType T>
			auto get() {
				// using V = MapEnumToType<T>::value;
				T* unit = std::get_if<T>(&value);
				return unit == nullptr ? std::nullopt : std::optional<T>{*unit};
			}

			template <Types::StyleSheetType T>
			constexpr bool isType() {
				// using V = MapEnumToType<T>::value;
				return std::holds_alternative<T>(this->value);
			}

			static StyleSheetValue fromString(ValueTypes type, std::string strValue);

		private:
			StyleSheetVariant value;
	};
} // namespace Radiant::StyleSheetParser
