#include "radiant/core/engine/StyleSheetStandardFunctions.h"
#include "radiant/css/values/Color.h"

namespace Radiant::StyleSheetStandardFunctions {

  StyleSheetParser::Color rgb(
      StyleSheetParser::Integer red, 
      StyleSheetParser::Integer green, 
      StyleSheetParser::Integer blue,
      StyleSheetParser::Integer alpha
  ) {
    return StyleSheetParser::Color(red, green, blue, alpha);
  }
}
