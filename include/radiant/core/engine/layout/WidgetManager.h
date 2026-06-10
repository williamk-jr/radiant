#pragma once
#include "radiant/core/engine/widgets/widget.h"
#include "radiant/core/render/Window.h"
#include "radiant/core/render/batch/RenderBatch.h"
#include <memory>
#include <unordered_map>

namespace Radiant {
  class WidgetManager {
    public:
      WidgetManager(Window& window);
      std::shared_ptr<Widget> getRootWidget();
      RenderBatch createRenderBatch();

    private:
      std::shared_ptr<Widget> rootWidget;
      //std::unordered_map<typename Key, typename Tp>
  };
}
