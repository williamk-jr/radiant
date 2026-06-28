#pragma once

#include <cstdint>
namespace Radiant {
  struct LayoutExtrusion {
    int32_t top = 0;
    int32_t bottom = 0;
    int32_t left = 0;
    int32_t right = 0;
  };

  class LayoutBox {
    public:
      LayoutBox();
      LayoutBox(int32_t positionX, int32_t positionY, uint32_t width, uint32_t height);
      
      int32_t getMarginBoxX() const;
      int32_t getMarginBoxY() const;

      int32_t getBorderBoxX() const;
      int32_t getBorderBoxY() const;

      int32_t getPaddingBoxX() const;
      int32_t getPaddingBoxY() const;

      int32_t getContentBoxX() const;
      int32_t getContentBoxY() const;

      int32_t getMarginBoxWidth() const;
      int32_t getMarginBoxHeight() const;

      int32_t getBorderBoxWidth() const;
      int32_t getBorderBoxHeight() const;

      int32_t getPaddingBoxWidth() const;
      int32_t getPaddingBoxHeight() const;

      int32_t getContentBoxWidth() const;
      int32_t getContentBoxHeight() const;

      void setPositionX(int32_t x);
      void setPositionY(int32_t y);

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

      LayoutExtrusion padding;
      LayoutExtrusion border;
      LayoutExtrusion margin;

      uint32_t offsetTop = 0;
      uint32_t offsetBottom = 0;
      uint32_t offsetLeft = 0;
      uint32_t offsetRight = 0;
  };
}
