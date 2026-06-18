#include "radiant/core/engine/LayoutBox.h"

namespace Radiant {
  
  LayoutBox::LayoutBox() {}

  LayoutBox::LayoutBox(uint32_t positionX, uint32_t positionY, uint32_t width, uint32_t height) : 
    positionX(positionX), positionY(positionY), width(width), height(height) {}
  
  uint32_t LayoutBox::getPositionX() {
    return this->positionX;
  }

  uint32_t LayoutBox::getPositionY() {
    return this->positionY;
  }

  uint32_t LayoutBox::getWidth() {
    return this->width;
  }

  uint32_t LayoutBox::getHeight() {
    return this->height;
  }

  void LayoutBox::offsetX(int32_t offset) {
    this->positionX += offset;
  }

  void LayoutBox::offsetY(int32_t offset) {
    this-> positionY += offset;
  }
}
