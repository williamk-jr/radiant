#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
namespace Radiant {
  class Color {
    public:
      Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
      Color(uint32_t hex);
      Color(std::string keyword);

      static Color fromString(std::string color);

    private:
      uint32_t rgba;
      static std::unordered_map<std::string, uint32_t> COLOR_KEYWORDS;
  };
}
