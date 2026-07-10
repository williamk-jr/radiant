#include "radiant/core/engine/font/FontManager.h"
#include "radiant/core/engine/font/Font.h"
#include <freetype/freetype.h>

namespace Radiant {
  FontManager::FontManager() {
    FT_Init_FreeType(&this->freetype);
  }
  FontManager::FontManager(FontManager&& other) noexcept :
    freetype(other.freetype) {
    other.freetype = nullptr;
  }

  FontManager::~FontManager() {
    FT_Done_FreeType(this->freetype);
  }

  Font FontManager::loadFont(std::filesystem::path path) {
    return {this->freetype, path};
  }
}
