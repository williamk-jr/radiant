#include "radiant/core/render/TextureAtlas.h"
#include "radiant/util/logger/Logger.h"
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace Radiant {
  TextureAtlas::TextureAtlas(uint32_t width, uint32_t height, uint32_t pixelSize) : 
    width(width), height(height), pixelSize(pixelSize) {
      this->buffer = std::vector<uint8_t>();
      this->buffer.reserve(width*height*pixelSize);
  }
  
  void TextureAtlas::addTexture(uint8_t* buffer, uint32_t size, uint32_t width, uint32_t height) {
    if (this->cursorX + width >= this->width) {
      if (this->cursorY + rowOffset >= this->height) {
        return; // We cannot go down another line.
      }

      this->cursorX = 0;
      this->cursorY += rowOffset;
      this->rowOffset = height;
    }

    //Logger::info(std::to_string(this->cursorX));
    for (int y = 0; y < height; y++) {
      uint8_t* src = buffer + (width * y * pixelSize);
      uint8_t* dst = this->buffer.data() + (this->cursorX * (this->cursorY + y) * pixelSize);

      std::copy(src, src+(width*pixelSize), dst);

    }
    cursorX += width*pixelSize;
  }
  
  uint8_t* TextureAtlas::getBuffer() {
    return this->buffer.data();
  }

  uint32_t TextureAtlas::getCursorX() const {
    return this->cursorX;
  }

  uint32_t TextureAtlas::getCursorY() const {
    return this->cursorY;
  }

  uint32_t TextureAtlas::getWidth() const {
    return this->width;
  }

  uint32_t TextureAtlas::getHeight() const {
    return this->height;
  }
  
  uint32_t TextureAtlas::getPixelSize() const {
    return this->pixelSize;
  }
}
