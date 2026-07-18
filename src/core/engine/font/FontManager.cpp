#include "radiant/core/engine/font/FontManager.h"
#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/font/FontCacheNode.h"
#include "radiant/core/engine/font/FontGPUCache.h"
#include "radiant/util/logger/Logger.h"
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/fttypes.h>
#include <memory>
#include <string>

namespace Radiant {
  FontManager::FontManager() {
    this->fontCache = std::make_unique<FontCache>(1024*4, FONT_CACHE_GLYPH | FONT_CACHE_SMALL_BITMAP);
    this->fontGpuCache = std::make_unique<FontGPUCache>();
  }

  Font FontManager::loadFont(std::filesystem::path path) {
    return {*this->fontCache, {path, 0}};
  }

  void FontManager::compileStringGeometry(Font font, std::string str) {
    FT_Size fontSize = this->fontCache->lookupPixelFontSize(font.fontFaceIdentifier, font.size, font.size);
    
    for (char charCode : str) {
      FontCacheNode<FT_Glyph> glyphNode = this->fontCache->lookupGlyph(font.fontFaceIdentifier, charCode, font.size, font.size);
      FT_BitmapGlyph bitmapGlyph = this->toBitmapGlyph(glyphNode.getValue(), FT_RENDER_MODE_NORMAL);
      
      GlyphIdentifier glyphId = {font.fontFaceIdentifier.faceId, (unsigned long)charCode};
      if (!fontGpuCache->hasEntry(glyphId)) {
        fontGpuCache->addEntry(bitmapGlyph->bitmap, glyphId);
      }
    }
  }
  
  FT_BitmapGlyph toBitmapGlyph(FT_Glyph glyph, FT_Render_Mode renderMode) {
    if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
      FT_Glyph_To_Bitmap(&glyph, renderMode, nullptr, false);
    }
    return (FT_BitmapGlyph)glyph;
  }
}
