#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/render/Rect2D.h"
#include "radiant/core/render/batch/RenderBatch.h"
#include "radiant/css/css_parser.h"
#include <memory>
#include <stack>

namespace Radiant {
  WidgetManager::WidgetManager(Window& window, StyleSheetParser::CssParser& styleSheetParser) : styleSheetParser(styleSheetParser) {
    Rect2D frameBufferSize = window.getFrameBufferSize();

    this->rootWidget = std::make_shared<Widget>(Widget{
        *this, 
        frameBufferSize.width, 
        frameBufferSize.height
    });
  }
  
  std::shared_ptr<Widget> WidgetManager::getRootWidget() {
    return this->rootWidget;
  }
  
  StyleSheetParser::CssParser& WidgetManager::getStyleSheetParser() {
    return this->styleSheetParser;
  }
  
  RenderBatch WidgetManager::createRenderBatch() {
    RenderBatch batch;

    std::stack<Widget*> widgets; 
    widgets.push(this->rootWidget.get());

    while (!widgets.empty()) {
      Widget* currentWidget = widgets.top();
      widgets.pop();

      batch.instances.push_back(
        {{255, 0, 0, 255}, {currentWidget->getPositionX(), currentWidget->getPositionY(), 0}, {200, 200}}
      );

      for (Widget* child : currentWidget->getChildren()) {
        widgets.push(child);
      }
    }
    return batch;
  }
}
