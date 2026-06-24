#pragma once

#include <cstdint>
namespace Radiant {
  class LayoutBox {
    public:
      LayoutBox();
      LayoutBox(uint32_t positionX, uint32_t positionY, uint32_t width, uint32_t height);
      
      uint32_t getPositionX();
      uint32_t getPositionY();

      uint32_t getWidth();
      uint32_t getHeight();

      void setPositionX(uint32_t x);
      void setPositionY(uint32_t y);

      void setWidth(uint32_t width);
      void setHeight(uint32_t height);

      void setTopOffset(uint32_t offset);
      void setBottomOffset(uint32_t offset);
      void setLeftOffset(uint32_t offset);
      void setRightOffset(uint32_t offset);

    private:
      uint32_t positionX = 0;
      uint32_t positionY = 0;
      uint32_t width = 0;
      uint32_t height = 0;

      uint32_t top = 0;
      uint32_t bottom = 0;
      uint32_t left = 0;
      uint32_t right = 0;
  };
}
