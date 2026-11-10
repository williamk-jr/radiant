#pragma once

namespace Radiant::StyleSheetParser::Types {
	class Float {
		public:
			Float();
			Float(float value);

			operator float() const {
				return this->value;
			}

		private:
			float value;
	};
} // namespace Radiant::StyleSheetParser::Types
