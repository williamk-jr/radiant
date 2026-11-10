#pragma once
#include <cstdint>

namespace Radiant::StyleSheetParser::Types {

	struct ResolutionContext {};

	template <typename T>
	class IValue {
		public:
			virtual T resolve(float referenceLength) = 0;
	};
} // namespace Radiant::StyleSheetParser::Types
