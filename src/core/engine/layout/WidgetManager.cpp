#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/engine/widgets/Unit.h"
#include "radiant/core/render/Rect2D.h"
#include <memory>

namespace Radiant {
  WidgetManager::WidgetManager(Window& window) {
    Rect2D frameBufferSize = window.getFrameBufferSize();

    //this->rootWidget = std::shared_ptr<Widget>(
    //    nullptr, 
    //    Unit{UnitType::PIXELS, frameBufferSize.width}, 
    //    Unit{UnitType::PIXELS, frameBufferSize.height}
    //);
  }
}
