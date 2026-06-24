#pragma once
#include "radiant/core/engine/widgets/widget.h"
#include "radiant/core/render/Window.h"
#include "radiant/core/render/batch/RenderBatch.h"
#include "radiant/css/Parser.h"
#include <memory>
#include <unordered_map>

namespace Radiant {
  class WidgetManager {
    public:
      WidgetManager(Window& window, StyleSheetParser::Parser& styleSheetParser);

      std::shared_ptr<Widget> getRootWidget();
      StyleSheetParser::Parser& getStyleSheetParser();
      RenderBatch createRenderBatch();

    private:
      StyleSheetParser::Parser& styleSheetParser;
      std::shared_ptr<Widget> rootWidget;
      //std::unordered_map<typename Key, typename Tp>
  };
}
