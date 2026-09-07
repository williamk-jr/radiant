#pragma once

#include <cstdint>

namespace Radiant {
	using ReturnCode = int32_t;

	template <typename T>
	class Result {
		public:
			ReturnCode getReturnCode() {
				return this->returnCode;
			}

			T getValue() {
				return this->value;
			}

		private:
			ReturnCode returnCode;
			T          value;
	};
} // namespace Radiant
