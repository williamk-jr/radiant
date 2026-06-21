#include "radiant/css/values/Integer.h"

namespace Radiant::StyleSheetParser {
  Integer::Integer() : value(0) {}
  Integer::Integer(int32_t value) : value(value) {}
}
