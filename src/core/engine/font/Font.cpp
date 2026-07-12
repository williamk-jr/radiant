#include "radiant/core/engine/font/Font.h"
#include "radiant/util/logger/Logger.h"
#include <cstring>
#include <freetype/freetype.h>
#include <freetype/ftimage.h>
#include <string>

namespace Radiant {
  Font::Font(FT_Library freetype, std::filesystem::path path) {
    const char* rawPath = path.c_str();
    FT_Error error = FT_New_Face(freetype, rawPath, 0, &this->fontFace);

    if (error) {
      Logger::error("Failed to load Font: "+path.string()+". Error code: " + std::to_string(error), {
        {"FONT", MessageStyle::WHITE},
      });
    } else {
      Logger::info("Loaded Font: "+path.string(), {
        {"FONT", MessageStyle::WHITE},
      }, 1);
    }
  }
  
  Font::Font(Font&& other) noexcept :
    fontFace(other.fontFace) {
    other.fontFace = nullptr;
  }

  Font::~Font() {
    FT_Done_Face(this->fontFace);
  }
  
  int Font::getNumCharmaps() {
    return this->fontFace->num_charmaps;
  }

  std::string Font::getFamilyName() {
    return this->fontFace->family_name;
  }

  std::string Font::getStyleName() {
    return this->fontFace->style_name;
  }
  
  bool Font::isScalable() {
    return FT_IS_SCALABLE(this->fontFace);
  }

  Bitmap Font::getBitmapFromCharCode(unsigned long charCode) {
    int error = 0;
    error = FT_Set_Char_Size(this->fontFace, 0, 16*64, 0, 72);

    FT_UInt glyphIndex = FT_Get_Char_Index(this->fontFace, charCode);
    error = FT_Load_Glyph(this->fontFace, glyphIndex, FT_LOAD_DEFAULT);
    error = FT_Render_Glyph(this->fontFace->glyph, FT_RENDER_MODE_NORMAL);

    if (error) {
      Logger::info(std::to_string(error));
    }
    FT_Bitmap glyphBitmap = this->fontFace->glyph->bitmap;

    size_t bufferSize = glyphBitmap.rows * glyphBitmap.pitch;
    size_t width = glyphBitmap.width;
    size_t height = glyphBitmap.rows;
    //Logger::info(std::to_string(this->fontFace->glyph->bitmap_left));
    //Logger::info(std::to_string(this->fontFace->glyph->bitmap_top));

    Bitmap bitmap(glyphBitmap.buffer, bufferSize, width, height);
    return bitmap; 
  }
  
  //void Font::createBitmaps() {
  //  FT_Load_Glyph(this->fontFace, 0, 0); // Load first glyph.
  //  FT_GlyphSlot glyph = this->fontFace->glyph;

  //  while (glyph != nullptr) {
  //    FT_Render_Glyph(glyph, FT_RENDER_MODE_MONO); // Load bitmap;
  //    FT_Bitmap bitmap = glyph->bitmap;

  //    


  //    glyph = this->fontFace->glyph->next; // Get next glyph;
  //  }


  //}
}
