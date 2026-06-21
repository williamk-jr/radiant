#include "radiant/core/engine/StyleSheetPropertyResolvers.h"
#include "radiant/util/logger/Logger.h"
#include <string>

namespace Radiant::PropertyResolvers {
  StyleSheetParser::StyleSheetEntry UNIT_LIST_RESOLVER(StyleSheetParser::StyleSheetEntry values, float context) {
    StyleSheetParser::StyleSheetEntry resolvedValues(values.size());
    //Logger::info("Size: " + std::to_string(values.size()));
    for (int i = 0; i < values.size(); i++) {
      StyleSheetParser::Unit relativeValue = values.get<StyleSheetParser::ValueTypes::UNIT>(i).value();
      //Logger::info("Value: " + std::to_string(relativeValue.getValue()));
      resolvedValues.add({relativeValue.resolve()});
    }

    return resolvedValues;
  }
}
