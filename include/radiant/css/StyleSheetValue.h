#pragma once

#include "radiant/css/values/Color.h"
#include "radiant/css/values/Float.h"
#include "radiant/css/values/Function.h"
#include "radiant/css/values/Integer.h"
#include "radiant/css/values/Unit.h"

#include <optional>
#include <string>
#include <variant>

namespace Radiant::StyleSheetParser {
	enum class ValueTypes { INTEGER, FLOAT, STRING, UNIT, COLOR, FUNCTION };

	typedef std::variant<std::string, Function, Integer, Float, Unit, Color> StyleSheetVariant;

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
	REGISTER_TYPE_MAP(Unit, ValueTypes::UNIT);
	REGISTER_TYPE_MAP(Integer, ValueTypes::INTEGER);
	REGISTER_TYPE_MAP(Float, ValueTypes::FLOAT);
	REGISTER_TYPE_MAP(Color, ValueTypes::COLOR);
	REGISTER_TYPE_MAP(Function, ValueTypes::FUNCTION);
#undef REGISTER_TYPE_MAP

	class StyleSheetValue {
		public:
			StyleSheetValue(StyleSheetVariant value);

			template <ValueTypes T>
			auto get() {
				using V = MapEnumToType<T>::value;
				V* unit = std::get_if<V>(&value);
				return unit == nullptr ? std::nullopt : std::optional<V>{*unit};
			}

			static StyleSheetValue fromString(ValueTypes type, std::string strValue);

		private:
			StyleSheetVariant value;
	};
} // namespace Radiant::StyleSheetParser
