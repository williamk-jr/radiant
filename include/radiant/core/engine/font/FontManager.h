#pragma once

#include "radiant/core/engine/font/Font.h"
#include <filesystem>

#include <ft2build.h>
#include <freetype/freetype.h>

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
    private:
      FT_Library freetype;
  };
}
