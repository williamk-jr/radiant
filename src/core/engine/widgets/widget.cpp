#include "radiant/core/engine/widgets/widget.h"

namespace Radiant {
  Widget::Widget(Widget* parent, Unit width, Unit height) :
    parent(parent), width(width), height(height) {
  }

  Widget::Widget(Widget* parent, uint32_t positionX, uint32_t positionY, Unit width, Unit height) :
    parent(parent), positionX(positionX), positionY(positionY), width(width), height(height) {

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

    for (Widget* child : this->children) {
      child->updateLayout();
    }
  }
}
