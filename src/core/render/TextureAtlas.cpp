#include "radiant/core/render/TextureAtlas.h"
#include "radiant/util/logger/Logger.h"
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace Radiant {
  TextureAtlas::TextureAtlas(uint32_t width, uint32_t height) : 
    width(width), height(height) {
      this->buffer = std::vector<uint8_t>();
      this->buffer.reserve(width*height*4);
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
      uint8_t* src = buffer + (width * y * 4);
      uint8_t* dst = this->buffer.data() + (this->cursorX * (this->cursorY + y) * 4);

      std::copy(src, src+(width*4), dst);

    }
    cursorX += width*4;
  }
  uint32_t TextureAtlas::getCursorX() {
    return this->cursorX;
  }

  uint32_t TextureAtlas::getCursorY() {
    return this->cursorY;
  }

  uint32_t TextureAtlas::getWidth() {
    return this->width;
  }

  uint32_t TextureAtlas::getHeight() {
    return this->height;
  }


}
