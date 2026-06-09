#include "radiant/core/engine/widgets/Unit.h"

namespace Radiant {

  Unit::Unit() :
    type(UnitType::PIXELS), value(0) {

  }
  
  Unit::Unit(UnitType type, uint32_t value) :
    type(type), value(value) {

  }
}
