#pragma once

#include <filesystem>

#include <ft2build.h>
#include <freetype/freetype.h>
namespace Radiant {
  class FontManager;

  struct FontBBox {
     
  };

  class Font {
    friend FontManager;

    public:
      Font(const Font&) = delete;
      Font& operator=(const Font&) = delete;

      Font(Font&&) noexcept;
      Font& operator=(Font&&) noexcept = default;
      ~Font();
      
      int getNumCharmaps();
      std::string getFamilyName();
      std::string getStyleName();

      bool isScalable();

    private:
      Font(FT_Library library, std::filesystem::path path);

      FT_Face fontFace;
  };
}
