#pragma once

#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/font/FontCache.h"
#include <filesystem>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace Radiant {
  class FontManager {
    public:
      FontManager();

      Font loadFont(std::filesystem::path path);
      Bitmap getBitmapFromCharCode(Font font, unsigned long charCode);
    private:
      std::unique_ptr<FontCache> fontCache;
  };
}
