#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/font/cache/FontCacheIdentifier.h"
#include "radiant/util/logger/Logger.h"
#include <cstring>
#include <freetype/freetype.h>
#include <freetype/ftimage.h>
#include <string>

namespace Radiant {
  Font::Font(FontCache& fontCache, FontCacheIdentifier fontFaceIdentifier) : 
    fontFaceIdentifier(fontFaceIdentifier) {
    this->fontFace = fontCache.lookupFontFace(fontFaceIdentifier);
  }
  
  Font::Font(const Font& other) : 
    fontFaceIdentifier(other.fontFaceIdentifier), fontFace(other.fontFace) {}
  
  Font::Font(Font&& other) noexcept :
    fontFaceIdentifier(other.fontFaceIdentifier), fontFace(other.fontFace) {
    other.fontFace = nullptr;
  }

  Font::~Font() {}

  void Font::setPointSize(uint32_t pointSize) {
    this->size = (pointSize * this->dpi) / 72;
  }

  void Font::setPixelSize(uint32_t pixelSize) {
    this->size = pixelSize;
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
}
