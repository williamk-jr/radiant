#include "radiant/core/engine/LayoutBox.h"

namespace Radiant {
  
  LayoutBox::LayoutBox() {
    this->padding = {10, 10, 10, 10};
    this->border = {10, 10, 10, 10};
    this->margin = {10, 10, 10, 10};
  }

  LayoutBox::LayoutBox(int32_t positionX, int32_t positionY, uint32_t width, uint32_t height) : 
    positionX(positionX), positionY(positionY), width(width), height(height) {
    this->padding = {10, 10, 10, 10};
    this->border = {1, 1, 1, 1};
    this->margin = {10, 10, 10, 10};
  }
  
  int32_t LayoutBox::getMarginBoxX() const {
    return this->getBorderBoxX() - margin.left; 
  }

  int32_t LayoutBox::getMarginBoxY() const {
    return this->getBorderBoxY() - margin.top; 
  }

  int32_t LayoutBox::getBorderBoxX() const {
    return this->getPaddingBoxX() - border.left;
  }

  int32_t LayoutBox::getBorderBoxY() const {
    return this->getPaddingBoxY() - border.top;
  }

  int32_t LayoutBox::getPaddingBoxX() const {
    return this->positionX + offsetLeft - offsetRight;
  }

  int32_t LayoutBox::getPaddingBoxY() const {
    return this->positionY + offsetTop - offsetBottom;
  }

  int32_t LayoutBox::getContentBoxX() const {
    return this->positionX + padding.left;
  }

  int32_t LayoutBox::getContentBoxY() const {
    return this->positionY + padding.top;
  }

  int32_t LayoutBox::getMarginBoxWidth() const {
    return this->getBorderBoxWidth() + margin.left + margin.right;
  }

  int32_t LayoutBox::getMarginBoxHeight() const {
    return this->getBorderBoxHeight() + margin.top + margin.bottom;
  }

  int32_t LayoutBox::getBorderBoxWidth() const {
    return this->getPaddingBoxWidth() + border.left + border.right;
  }

  int32_t LayoutBox::getBorderBoxHeight() const {
    return this->getPaddingBoxHeight() + border.top + border.bottom;
  }

  int32_t LayoutBox::getPaddingBoxWidth() const {
    return this->width;
  }

  int32_t LayoutBox::getPaddingBoxHeight() const {
    return this->height; 
  }

  int32_t LayoutBox::getContentBoxWidth() const {
    return this->width - padding.left - padding.right;
  }

  int32_t LayoutBox::getContentBoxHeight() const {
    return this->height - padding.top - padding.bottom;
  }

  void LayoutBox::setPositionX(int32_t x) {
    this->positionX = x;
  }

  void LayoutBox::setPositionY(int32_t y) {
    this->positionY = y;
  }

  void LayoutBox::setWidth(uint32_t width) {
    this->width = width;
  }

  void LayoutBox::setHeight(uint32_t height) {
    this->height = height;
  }

  void LayoutBox::setTopOffset(uint32_t offset) {
    this->offsetTop = offset;
  }

  void LayoutBox::setBottomOffset(uint32_t offset) {
    this->offsetBottom = offset;
  }

  void LayoutBox::setLeftOffset(uint32_t offset) {
    this->offsetLeft = offset;
  }

  void LayoutBox::setRightOffset(uint32_t offset) {
    this->offsetRight = offset;
  }
}
