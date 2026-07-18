#pragma once

#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/font/cache/FontCache.h"
#include "radiant/core/engine/font/cache/FontGPUCache.h"
#include <filesystem>

#include <freetype/ftglyph.h>
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
      void compileStringGeometry(Font font, std::string str);
    private:
      std::unique_ptr<FontCache> fontCache;
      std::unique_ptr<FontGPUCache> fontGpuCache;
      FT_BitmapGlyph toBitmapGlyph(FT_Glyph glyph, FT_Render_Mode renderMode);
  };
}
