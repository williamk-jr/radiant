#pragma once

#include "radiant/core/engine/font/Bitmap.h"
#include <filesystem>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <vector>
namespace Radiant {
  class FontManager;

  class Font {
    friend FontManager;

    public:
      Font(const Font&) = delete;
      Font& operator=(const Font&) = delete;

      Font(Font&&) noexcept;
      Font& operator=(Font&&) noexcept = default;
      ~Font();
      
      Bitmap getBitmapFromCharCode(unsigned long charCode);
      
      int getNumCharmaps();
      std::string getFamilyName();
      std::string getStyleName();

      bool isScalable();

    private:
      Font(FT_Library library, std::filesystem::path path);

      FT_Face fontFace;
  };
}
