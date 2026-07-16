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
      FontManager(const FontManager&) = delete;
      FontManager& operator=(const FontManager&) = delete;

      FontManager(FontManager&&) noexcept;
      FontManager& operator=(FontManager&&) noexcept = default;
      ~FontManager();

      Font loadFont(std::filesystem::path path);
      Bitmap getBitmapFromCharCode(Font font, unsigned long charCode);
    private:
      FT_Library freetype;
      std::unique_ptr<FontCache> fontCache;
  };
}
