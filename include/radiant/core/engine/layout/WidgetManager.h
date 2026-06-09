#pragma once
#include "radiant/core/engine/widgets/widget.h"
#include "radiant/core/render/Window.h"
#include <memory>

namespace Radiant {
  class WidgetManager {
    public:
      WidgetManager(Window& window);

    private:
      std::shared_ptr<Widget> rootWidget;
  };
}
