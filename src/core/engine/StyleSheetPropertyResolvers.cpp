#include "radiant/core/engine/StyleSheetPropertyResolvers.h"
#include "radiant/util/logger/Logger.h"
#include <string>

namespace Radiant::PropertyResolvers {
  StyleSheetEntry UNIT_LIST_RESOLVER(StyleSheetEntry values, float context) {
    StyleSheetEntry resolvedValues(values.size());
    //Logger::info("Size: " + std::to_string(values.size()));
    for (int i = 0; i < values.size(); i++) {
      Unit relativeValue = values.get<StyleSheetValueTypes::UNIT>(i).value();
      //Logger::info("Value: " + std::to_string(relativeValue.getValue()));
      resolvedValues.add({relativeValue.resolve()});
    }

    return resolvedValues;
  }
}
