#pragma once
#include "radiant/core/engine/font/FontCacheNode.h"
#include <cstdint>
#include <filesystem>
#include <freetype/freetype.h>
#include <freetype/ftcache.h>
#include <freetype/fttypes.h>

namespace Radiant {
  using FontCacheType = uint8_t;

  enum FontCacheTypes: uint8_t {
    FONT_CACHE_GLYPH         = 0x01,
    FONT_CACHE_SMALL_BITMAP  = 0x02
  };

  struct FontFaceId {
    std::filesystem::path path;
    uint32_t faceId;
  };

  class FontCache {
    public:
      FontCache(size_t cacheSize, FontCacheType cacheType);
      FontCache(const FontCache&);
      FontCache& operator=(const FontCache&) = default;

      FontCache(FontCache&&) noexcept;
      FontCache& operator=(FontCache&&) noexcept = default;
      ~FontCache();

      FT_Face lookupFontFace(FontFaceId fontFaceId);
      FontCacheNode<FT_Glyph> lookupGlyph(FontFaceId faceIdentifier, unsigned long charCode, int width, int height);
      FontCacheNode<FTC_SBit> lookupBitmap(FontFaceId faceIdentifier, unsigned long charCode, int width, int height);

    private:
      FT_Library freetype;
      FTC_Manager cacheManager;
      FTC_CMapCache charMapCache;

      FTC_ImageCache glyphImageCache;
      FTC_SBitCache smallBitmapCache;

      FontCacheType cacheType;


      static FT_Error requestFontFace(FTC_FaceID faceIdentifier, FT_Library freetype, FT_Pointer requestData, FT_Face* fontFace);
  };
}
