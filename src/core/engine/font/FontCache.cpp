#include "radiant/core/engine/font/FontCache.h"
#include "radiant/util/logger/Logger.h"
#include <cstddef>
#include <freetype/freetype.h>
#include <freetype/ftcache.h>
#include <freetype/ftglyph.h>
#include <freetype/fttypes.h>
#include <string>

namespace Radiant {
  FontCache::FontCache(size_t cacheSize, FontCacheType cacheType) {
    FT_Error error = FT_Init_FreeType(&this->freetype);
    if (error) {
      Logger::error("Failed to load Font Manager, error code: " + std::to_string(error), {
        {"FONT", MessageStyle::WHITE},
      });
    } else {
      Logger::info("Loaded Font Manager.", {
        {"FONT", MessageStyle::WHITE},
      }, 1);
    }


    FTC_Manager_New(
        freetype, 
        1024, 1024, 
        cacheSize,
        FontCache::requestFontFace, 
        nullptr,
        &this->cacheManager
    );

    FTC_CMapCache_New(this->cacheManager, &this->charMapCache);

    if (cacheType & FONT_CACHE_GLYPH) {
      FTC_ImageCache_New(this->cacheManager, &this->glyphImageCache);
    }

    if (cacheType & FONT_CACHE_SMALL_BITMAP) {
      FTC_ImageCache_New(this->cacheManager, &this->glyphImageCache);
    }
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
    FT_Done_FreeType(this->freetype);
  }
  
  FT_Face FontCache::lookupFontFace(FontFaceId fontFaceId) {
    FT_Face fontFace;
    FT_Error error = FTC_Manager_LookupFace(this->cacheManager, &fontFaceId, &fontFace);
    Logger::info(fontFace->family_name);
    return fontFace;
  }

  FontCacheNode<FT_Glyph> FontCache::lookupGlyph(FontFaceId faceIdentifier, unsigned long charCode, int width, int height) {
    FTC_ImageType imageType{};
    imageType->face_id = &faceIdentifier;
    imageType->width = width;
    imageType->height = height;
    imageType->flags = FT_LOAD_DEFAULT;
    
    FT_Glyph glyph;
    FTC_Node cacheNode;
    FT_UInt gindex = FTC_CMapCache_Lookup(this->charMapCache, &faceIdentifier, -1, charCode);
    FTC_ImageCache_Lookup(this->glyphImageCache, imageType, gindex, &glyph, &cacheNode);
    return {this->cacheManager, glyph, cacheNode};
  }

  FontCacheNode<FTC_SBit> FontCache::lookupBitmap(FontFaceId faceIdentifier, unsigned long charCode, int width, int height) {
    FTC_ImageType imageType{};
    imageType->face_id = &faceIdentifier;
    imageType->width = width;
    imageType->height = height;
    imageType->flags = FT_LOAD_DEFAULT;

    FTC_SBit smallBitmap;
    FTC_Node cacheNode;
    FT_UInt gindex = FTC_CMapCache_Lookup(this->charMapCache, &faceIdentifier, -1, charCode);
    FTC_SBitCache_Lookup(this->smallBitmapCache, imageType, gindex, &smallBitmap, &cacheNode);
    return {this->cacheManager, smallBitmap, cacheNode};
  }
  
  FT_Error FontCache::requestFontFace(FTC_FaceID faceIdentifier, FT_Library freetype, FT_Pointer requestData, FT_Face* fontFace) {
    FontFaceId* faceId = static_cast<FontFaceId*>(faceIdentifier); 
    const char* rawPath = faceId->path.c_str();

    return FT_New_Face(freetype, rawPath, faceId->faceId, fontFace);
  }
}
