#pragma once

#include "radiant/css/values/Color.h"
#include "radiant/css/values/Float.h"
#include "radiant/css/values/Integer.h"
namespace Radiant::StyleSheetStandardFunctions {
  StyleSheetParser::Color rgb(
      StyleSheetParser::Integer red, 
      StyleSheetParser::Integer green, 
      StyleSheetParser::Integer blue,
      StyleSheetParser::Integer alpha
  );
}
