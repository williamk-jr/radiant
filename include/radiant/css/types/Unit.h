#pragma once
#include <string>
#include <unordered_map>

namespace Radiant::StyleSheetParser::Types {
	enum class UnitType { PIXEL, PERCENTAGE };

	class Unit {
		public:
			Unit(float value, UnitType unit);

			float    getValue();
			UnitType getUnit();

			Unit resolve(float referenceLength);

			static Unit fromString(std::string str);

		private:
			float    value;
			UnitType unit;

			inline static const std::unordered_map<std::string, UnitType> UNIT_MAP{{"%", UnitType::PERCENTAGE},
			                                                                       {"px", UnitType::PIXEL}};
	};
} // namespace Radiant::StyleSheetParser::Types
