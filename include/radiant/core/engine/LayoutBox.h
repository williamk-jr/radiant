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

      void offsetX(int32_t offset);
      void offsetY(int32_t offset);

    private:
      uint32_t positionX;
      uint32_t positionY;
      uint32_t width;
      uint32_t height;
  };
}
