#include "radiant/core/engine/font/FontCache.h"
#include <cstddef>
#include <freetype/freetype.h>
#include <freetype/ftcache.h>
#include <freetype/ftglyph.h>
#include <freetype/fttypes.h>

namespace Radiant {
  FontCache::FontCache(FT_Library freetype, size_t cacheSize, FontCacheType cacheType) {
    FTC_Manager_New(
        freetype, 
        1024, 1024, 
        cacheSize,
        FontCache::requestFontFace, 
        nullptr,
        &this->cacheManager
    );

    // TODO init caches.
  }
  
  FontCache::FontCache(FontCache&& other) noexcept :
    cacheManager(other.cacheManager), 
    glyphImageCache(other.glyphImageCache), 
    smallBitmapCache(other.smallBitmapCache),
    cacheType(other.cacheType) {

    other.cacheManager = nullptr;
    other.glyphImageCache = nullptr;
    other.smallBitmapCache = nullptr;
  }

  FontCache::~FontCache() {
    FTC_Manager_Done(this->cacheManager);
  }

  void FontCache::lookupGlyph(FontFaceId faceIdentifier, unsigned long charCode, int width, int height) {
    FTC_ImageType imageType{};
    imageType->face_id = &faceIdentifier;
    imageType->width = width;
    imageType->height = height;
    imageType->flags = FT_LOAD_DEFAULT;

    FT_Face fontFace;
    FTC_Manager_LookupFace(this->cacheManager, &faceIdentifier, &fontFace);

    FT_Glyph glyph;
    FTC_Node cacheNode;
    FT_UInt gindex = FT_Get_Char_Index(fontFace, charCode);
    FTC_ImageCache_Lookup(this->glyphImageCache, imageType, gindex, &glyph, &cacheNode);
  }

  void FontCache::lookupBitmap(FontFaceId faceIdentifier, unsigned long charCode, int width, int height) {
    FTC_ImageType imageType{};
    imageType->face_id = &faceIdentifier;
    imageType->width = width;
    imageType->height = height;
    imageType->flags = FT_LOAD_DEFAULT;

    FT_Face fontFace;
    FTC_Manager_LookupFace(this->cacheManager, &faceIdentifier, &fontFace);

    FTC_SBit smallBitmap;
    FTC_Node cacheNode;
    FT_UInt gindex = FT_Get_Char_Index(fontFace, charCode);
    FTC_SBitCache_Lookup(this->smallBitmapCache, imageType, gindex, &smallBitmap, &cacheNode);
  }
  
  FT_Error FontCache::requestFontFace(FTC_FaceID faceIdentifier, FT_Library freetype, FT_Pointer requestData, FT_Face* fontFace) {
    FontFaceId* faceId = static_cast<FontFaceId*>(faceIdentifier); 
    const char* rawPath = faceId->path.c_str();

    return FT_New_Face(freetype, rawPath, faceId->faceId, fontFace);
  }
}
