#pragma once

#include <functional>
#include <string>
#include <vector>

namespace Radiant::StyleSheetParser {
	class StyleSheetValue;

	namespace Types {

		using RuntimeFunction = std::function<StyleSheetValue(std::vector<StyleSheetValue>)>;

		class Function {
			public:
				Function(std::string name, RuntimeFunction function, std::vector<StyleSheetValue> parameters);

				StyleSheetValue resolve(float referenceLength);

			private:
				std::string                  name;
				RuntimeFunction              function;
				std::vector<StyleSheetValue> parameters;
		};
	} // namespace Types
} // namespace Radiant::StyleSheetParser
