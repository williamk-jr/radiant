#include "radiant/css/values/Function.h"
#include "radiant/css/StyleSheetValue.h"

namespace Radiant::StyleSheetParser {
  Function::Function(std::string name, std::vector<StyleSheetValue> parameters) : 
    name(name), parameters(parameters) {}
}
