#pragma once

#include <cstdint>
#include <vector>
namespace Radiant {
  class TextureAtlas {
    public:
      TextureAtlas(uint32_t width, uint32_t height);
      void addTexture(uint8_t* buffer, uint32_t size, uint32_t width, uint32_t height);

      uint32_t getCursorX();
      uint32_t getCursorY();

      uint32_t getWidth();
      uint32_t getHeight();

    private:
      std::vector<uint8_t> buffer;
      uint32_t width;
      uint32_t height;

      uint32_t cursorX;
      uint32_t cursorY;
      uint32_t rowOffset;
  };
}
