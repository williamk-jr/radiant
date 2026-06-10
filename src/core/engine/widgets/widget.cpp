#include "radiant/core/engine/widgets/widget.h"
#include <memory>

namespace Radiant {
  Widget::Widget(std::shared_ptr<Widget> parent, Unit width, Unit height) :
    parent(parent), width(width), height(height) {
    if (parent != nullptr) {
      parent->addChild(this);
    }
  }

  Widget::Widget(std::shared_ptr<Widget> parent, uint32_t positionX, uint32_t positionY, Unit width, Unit height) :
    parent(parent), positionX(positionX), positionY(positionY), width(width), height(height) {
    if (parent != nullptr) {
      parent->addChild(this);
    }
  }

  uint32_t Widget::getPositionX() {
    return 0;
  }
  uint32_t Widget::getPositionY() {

    return 0;
  }
  
  uint32_t Widget::getWidth() {

    return 0;
  }
  uint32_t Widget::getHeight() {

    return 0;
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
