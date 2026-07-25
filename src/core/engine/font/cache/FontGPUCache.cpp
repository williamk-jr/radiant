#include "radiant/core/engine/font/cache/FontGPUCache.h"
#include "radiant/core/render/TextureAtlas.h"
#include "radiant/util/Box.h"
#include "radiant/util/logger/Logger.h"
#include <freetype/ftglyph.h>
#include <memory>
#include <string>
#include <utility>

namespace Radiant {
  FontGPUCache::FontGPUCache() {
    this->textureAtlas = std::make_unique<TextureAtlas>(2048, 2048, 1);
  }

  void FontGPUCache::addEntry(FT_Bitmap& bitmap, GlyphIdentifier identifier) {
    size_t bufferSize = bitmap.rows * bitmap.pitch;
    size_t width = bitmap.width;
    size_t height = bitmap.rows;

    Box uvBounds = this->textureAtlas->getUVBoundsAtCursor(width, height);
    this->textureAtlas->addTexture(bitmap.buffer, bufferSize, width, height);

    Logger::info("Char: "+std::to_string((char)identifier.charCode)+
                  "\nMin: ("+std::to_string(uvBounds.minX)+", "+std::to_string(uvBounds.minY)+
                  "\nMax: ("+std::to_string(uvBounds.maxX)+", "+std::to_string(uvBounds.maxY) +"\n");

    this->cache[identifier] = uvBounds;
    this->cacheDirty = true;
  }

  bool FontGPUCache::hasEntry(GlyphIdentifier identifier) {
    return this->cache.find(identifier) != this->cache.end();
  }

  bool FontGPUCache::isDirty() {
    return this->cacheDirty;
  }

  TextureAtlas& FontGPUCache::getTextureAtlas() {
    return *this->textureAtlas;
  }
  
  std::pair<float, float> FontGPUCache::calculateUV(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    return {x/width, y/height};
  }
}
