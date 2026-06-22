#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/render/Rect2D.h"
#include "radiant/core/render/batch/RenderBatch.h"
#include "radiant/css/Parser.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/values/Color.h"
#include <memory>
#include <stack>
#include <string>

namespace Radiant {
  WidgetManager::WidgetManager(Window& window, StyleSheetParser::Parser& styleSheetParser) : styleSheetParser(styleSheetParser) {
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
  
  StyleSheetParser::Parser& WidgetManager::getStyleSheetParser() {
    return this->styleSheetParser;
  }
  
  RenderBatch WidgetManager::createRenderBatch() {
    RenderBatch batch;

    std::stack<Widget*> widgets; 
    widgets.push(this->rootWidget.get());

    while (!widgets.empty()) {
      Widget* currentWidget = widgets.top();
      widgets.pop();
      
      StyleSheetParser::StyleSheetEntry backgroundColorEntry = 
        currentWidget->getStyle("background-color", {{ StyleSheetParser::Color(255,0,0,255) }});
      StyleSheetParser::Color backgroundColor = backgroundColorEntry.get<StyleSheetParser::ValueTypes::COLOR>(0).value();

      batch.instances.push_back({
        {backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha()}, 
        {currentWidget->getPositionX(), currentWidget->getPositionY(), 0}, 
        {currentWidget->getWidth(), currentWidget->getHeight()}
      });

      for (Widget* child : currentWidget->getChildren()) {
        widgets.push(child);
      }
    }
    return batch;
  }
}
