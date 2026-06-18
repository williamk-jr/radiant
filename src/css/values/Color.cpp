#include "radiant/css/values/Color.h"
#include "radiant/util/string_util.h"
#include <string>

namespace Radiant::LibStyleSheets {
  std::unordered_map<std::string, Colors> Color::COLOR_KEYWORDS{
    {"red", Colors::RED},
    {"green", Colors::GREEN},
    {"blue", Colors::BLUE},
    {"black", Colors::BLACK},
    {"white", Colors::WHITE},
  };
  
  Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    this->rgba = red;
    this->rgba = (this->rgba << 8) | green;
    this->rgba = (this->rgba << 16) | blue;
    this->rgba = (this->rgba << 24) | alpha;
  }

  Color::Color(uint32_t hex) {
    this->rgba = hex;
  }

  Color::Color(Colors keyword) {
    this->rgba = static_cast<uint32_t>(keyword); 
  }

  uint8_t Color::getRed() {
    return (this->rgba & 0xFF000000) >> 24;
  }

  uint8_t Color::getGreen() {
    return (this->rgba & 0x00FF0000) >> 16;
  }

  uint8_t Color::getBlue() {
    return (this->rgba & 0x0000FF00) >> 8;
  }

  uint8_t Color::getAlpha() {
    return this->rgba & 0x000000FF;
  }

  Color Color::fromString(std::string color) {
    if (Color::COLOR_KEYWORDS.find(color) != Color::COLOR_KEYWORDS.end()) {
      return Color(Color::COLOR_KEYWORDS[color]);
    } else if (string_util::isHexadecimal(color)) {
      return Color(std::stoul("0x"+color.substr(1, color.size()), nullptr, 16));
    }
  };
}
