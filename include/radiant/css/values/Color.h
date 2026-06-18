#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
namespace Radiant::LibStyleSheets {
  enum class Colors : uint32_t {
    RED=0xFF0000FF,
    GREEN=0x00FF00FF,
    BLUE=0x0000FFFF,
    BLACK=0x000000FF,
    WHITE=0xFFFFFFFF
  };

  class Color {
    public:
      Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
      Color(uint32_t hex);
      Color(Colors keyword);

      uint8_t getRed();
      uint8_t getGreen();
      uint8_t getBlue();
      uint8_t getAlpha();

      static Color fromString(std::string color);

    private:
      uint32_t rgba;
      static std::unordered_map<std::string, Colors> COLOR_KEYWORDS;
  };
}
