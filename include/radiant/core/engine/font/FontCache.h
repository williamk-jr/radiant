#pragma once
#include <cstdint>
#include <filesystem>
#include <freetype/freetype.h>
#include <freetype/ftcache.h>
#include <freetype/fttypes.h>

namespace Radiant {

  enum class FontCacheType {
    GLYPH,
    SMALL_BITMAP,
    BOTH
  };

  struct FontFaceId {
    std::filesystem::path path;
    uint32_t faceId;
  };

  class FontCache {
    public:
      FontCache(FT_Library freetype, size_t cacheSize, FontCacheType cacheType);
      FontCache(const FontCache&);
      FontCache& operator=(const FontCache&) = default;

      FontCache(FontCache&&) noexcept;
      FontCache& operator=(FontCache&&) noexcept = default;
      ~FontCache();

      void lookupGlyph(FontFaceId faceIdentifier, unsigned long charCode, int width, int height);
      void lookupBitmap(FontFaceId faceIdentifier, unsigned long charCode, int width, int height);

    private:
      FTC_Manager cacheManager;
      FTC_ImageCache glyphImageCache;
      FTC_SBitCache smallBitmapCache;

      FontCacheType cacheType;


      static FT_Error requestFontFace(FTC_FaceID faceIdentifier, FT_Library freetype, FT_Pointer requestData, FT_Face* fontFace);
  };
}
