#pragma once

#include "radiant/core/engine/font/Bitmap.h"
#include "radiant/core/engine/font/FontCache.h"
#include <filesystem>

#include <freetype/ftcache.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <vector>
namespace Radiant {
  class FontManager;

  class Font {
    friend FontManager;

    public:
      Font(const Font&);
      Font& operator=(const Font&) = default;

      Font(Font&&) noexcept;
      Font& operator=(Font&&) noexcept = default;
      ~Font();
      
      Bitmap getBitmapFromCharCode(unsigned long charCode);
      
      int getNumCharmaps();
      std::string getFamilyName();
      std::string getStyleName();

      bool isScalable();

    private:
      Font(FontCache& fontCache, FontFaceId fontFaceIdentifier);
      FontFaceId fontFaceIdentifier;
      FT_Face fontFace;
      size_t size;
  };
}
