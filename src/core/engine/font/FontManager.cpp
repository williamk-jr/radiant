#include "radiant/core/engine/font/FontManager.h"
#include "radiant/core/engine/font/Font.h"
#include "radiant/util/logger/Logger.h"
#include <freetype/freetype.h>
#include <freetype/fttypes.h>
#include <memory>
#include <string>

namespace Radiant {
  FontManager::FontManager() {
    this->fontCache = std::make_unique<FontCache>(1024*4, FONT_CACHE_GLYPH | FONT_CACHE_SMALL_BITMAP);
  }

  Font FontManager::loadFont(std::filesystem::path path) {
    return {*this->fontCache, {path, 0}};
  }
}
