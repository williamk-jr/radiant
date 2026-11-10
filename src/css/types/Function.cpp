#include "radiant/css/types/Function.h"

#include "radiant/css/StyleSheetValue.h"

namespace Radiant::StyleSheetParser::Types {
	Function::Function(std::string name, RuntimeFunction function, std::vector<StyleSheetValue> parameters)
	    : name(name), function(function), parameters(parameters) {}

	StyleSheetValue Function::resolve(float referenceLength) {
		std::vector<StyleSheetValue> resolvedParameters;

		for (int i = 0; i < name.length(); i++) {
			if (this->parameters[i].isType<ValueTypes::FUNCTION>()) {
				std::optional<Function> func = this->parameters[i].get<ValueTypes::FUNCTION>();
				resolvedParameters.push_back(func.value().resolve(referenceLength));
			} else {
				resolvedParameters.push_back(this->parameters[i]);
			}
		}

		return this->function(resolvedParameters);
	}
} // namespace Radiant::StyleSheetParser::Types
