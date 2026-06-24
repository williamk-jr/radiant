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
    this->layoutManager = std::make_unique<LayoutManager>();
    this->updateLayout();
  }

  Widget::Widget(std::shared_ptr<Widget> parent, uint32_t positionX, uint32_t positionY, uint32_t width, uint32_t height) :
    parent(parent), manager(parent->manager), layoutBox(positionX, positionY, width, height) {
    if (parent != nullptr) {
      parent->addChild(this);
    }
    this->layoutManager = std::make_unique<LayoutManager>();
    this->updateLayout();
  }
  
  Widget::Widget(WidgetManager& manager, uint32_t width, uint32_t height) : 
    parent(nullptr), manager(manager), layoutBox(0, 0, width, height) {
    this->layoutManager = std::make_unique<LayoutManager>();
    this->updateLayout();
  }
  
  void Widget::addStyle(std::string name, StyleSheetParser::StyleSheetEntry entry) {
    this->styleSheet.add(name, entry);
    this->updateLayout();
  }
  
  StyleSheetParser::StyleSheetEntry Widget::getStyle(std::string name, StyleSheetParser::StyleSheetEntry defaultEntry) {
    return this->styleSheet.getOrDefault(name, defaultEntry);
  }
  
  uint32_t Widget::getPositionX() {
    return this->layoutBox.getPositionX();
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
  
  LayoutManager& Widget::getLayoutManager() {
    return *this->layoutManager;
  }

  void Widget::setPositionX(uint32_t x) {
    this->layoutBox.setPositionX(x);
  }

  void Widget::setPositionY(uint32_t y) {
    this->layoutBox.setPositionY(y);
  }

  void Widget::setWidth(uint32_t width) {
    this->layoutBox.setWidth(width); 
  }

  void Widget::setHeight(uint32_t height) {
    this->layoutBox.setHeight(height);
  }

  void Widget::setTopOffset(uint32_t offset) {
    this->layoutBox.setTopOffset(offset);
  }

  void Widget::setBottomOffset(uint32_t offset) {
    this->layoutBox.setBottomOffset(offset);
  }

  void Widget::setLeftOffset(uint32_t offset) {
    this->layoutBox.setLeftOffset(offset);
  }

  void Widget::setRightOffset(uint32_t offset) {
    this->layoutBox.setRightOffset(offset);
  }

  std::vector<Widget*> Widget::getChildren() {
    return this->children;
  }
  
  std::shared_ptr<Widget> Widget::getParent() {
    return this->parent;
  }
  
  void Widget::addChild(Widget* child) {
    this->children.push_back(child);
  }

  void Widget::render() {
    
  }

  void Widget::updateLayout() {
    this->layoutManager->updateLayout(this);
  }
}
