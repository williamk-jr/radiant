#include "radiant/core/engine/widgets/widget.h"
#include "radiant/core/engine/LayoutBox.h"
#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/values/Unit.h"
#include <memory>

namespace Radiant {
  Widget::Widget(std::shared_ptr<Widget> parent, uint32_t width, uint32_t height) :
    parent(parent), manager(parent->manager), layoutBox(0, 0, width, height) {
    if (parent != nullptr) {
      parent->addChild(this);
    }
  }

  Widget::Widget(std::shared_ptr<Widget> parent, uint32_t positionX, uint32_t positionY, uint32_t width, uint32_t height) :
    parent(parent), manager(parent->manager), layoutBox(positionX, positionY, width, height) {
    if (parent != nullptr) {
      parent->addChild(this);
    }
  }
  
  Widget::Widget(WidgetManager& manager, uint32_t width, uint32_t height) : 
    parent(nullptr), manager(manager), layoutBox(0, 0, width, height) {

  }
  
  void Widget::addStyle(std::string name, StyleSheetEntry entry) {
    this->styleSheet.add(name, entry);
  }
  
  uint32_t Widget::getPositionX() {
    Unit right = this->styleSheet.getAbsolute(this->manager.getStyleSheetParser(), "right").get<StyleSheetValueTypes::UNIT>(0).value();
    Unit left = this->styleSheet.getAbsolute(this->manager.getStyleSheetParser(), "left").get<StyleSheetValueTypes::UNIT>(0).value();
    return this->layoutBox.getPositionX() + right.getValue() - left.getValue();
  }

  uint32_t Widget::getPositionY() {
    return this->layoutBox.getPositionY();
  }
  
  uint32_t Widget::getWidth() {
    return this->layoutBox.getWidth();
  }

  uint32_t Widget::getHeight() {
    return this->layoutBox.getHeight();
  }

  std::vector<Widget*> Widget::getChildren() {
    return this->children;
  }
  
  void Widget::addChild(Widget* child) {
    this->children.push_back(child);
  }

  void Widget::render() {

  }

  void Widget::updateLayout() {
    uint32_t startX = 0;
    uint32_t startY = 0;

    switch (this->positionType) {
      case PositionType::RELATIVE:
        //startX = this->parent.getPositionX();
        //startY = this->parent.getPositionY();

        break;
      case PositionType::ABSOLUTE:

        break;
    }
  }
}
