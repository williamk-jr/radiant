#include "radiant/core/engine/LayoutBox.h"

namespace Radiant {
  
  LayoutBox::LayoutBox() {}

  LayoutBox::LayoutBox(uint32_t positionX, uint32_t positionY, uint32_t width, uint32_t height) : 
    positionX(positionX), positionY(positionY), width(width), height(height) {}
  
  uint32_t LayoutBox::getPositionX() {
    return this->positionX + this->left - this->right;
  }

  uint32_t LayoutBox::getPositionY() {
    return this->positionY + this->top - this->bottom;
  }

  uint32_t LayoutBox::getWidth() {
    return this->width;
  }

  uint32_t LayoutBox::getHeight() {
    return this->height;
  }

  void LayoutBox::setPositionX(uint32_t x) {
    this->positionX = x;
  }

  void LayoutBox::setPositionY(uint32_t y) {
    this->positionY = y;
  }

  void LayoutBox::setWidth(uint32_t width) {
    this->width = width;
  }

  void LayoutBox::setHeight(uint32_t height) {
    this->height = height;
  }

  void LayoutBox::setTopOffset(uint32_t offset) {
    this->top = offset;
  }

  void LayoutBox::setBottomOffset(uint32_t offset) {
    this->bottom = offset;
  }

  void LayoutBox::setLeftOffset(uint32_t offset) {
    this->left = offset;
  }

  void LayoutBox::setRightOffset(uint32_t offset) {
    this->right = offset;
  }
}
