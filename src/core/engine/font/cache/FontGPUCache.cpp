#include "radiant/core/engine/font/cache/FontGPUCache.h"
#include "radiant/core/render/TextureAtlas.h"
#include <freetype/ftglyph.h>
#include <memory>
#include <utility>

namespace Radiant {
  // TODO Write custom hash function for glyph identifiers.
  FontGPUCache::FontGPUCache() {
    this->textureAtlas = std::make_unique<TextureAtlas>(2048, 2048);
  }

  void FontGPUCache::addEntry(FT_Bitmap& bitmap, GlyphIdentifier identifier) {
    size_t bufferSize = bitmap.rows * bitmap.pitch;
    size_t width = bitmap.width;
    size_t height = bitmap.rows;

    std::pair<float, float> uvMin = this->calculateUV(
        this->textureAtlas->getCursorX(), 
        this->textureAtlas->getCursorY(), 
        this->textureAtlas->getWidth(), 
        this->textureAtlas->getHeight()
    );

    this->textureAtlas->addTexture(bitmap.buffer, bufferSize, width, height);

    std::pair<float, float> uvMax = this->calculateUV(
        this->textureAtlas->getCursorX(), 
        this->textureAtlas->getCursorY(), 
        this->textureAtlas->getWidth(), 
        this->textureAtlas->getHeight()
    );

    // Stores default value, actual value is not generated until retrieval of texture atlas.
    this->cache[identifier] = {uvMin.first, uvMin.second, uvMax.first, uvMax.second};
    this->cacheDirty = true;
  }

  bool FontGPUCache::hasEntry(GlyphIdentifier identifier) {
    return this->cache.find(identifier) != this->cache.end();
  }

  bool FontGPUCache::isDirty() {
    return this->cacheDirty;
  }

  const TextureAtlas& FontGPUCache::getTextureAtlas() {
    if (!this->isDirty()) {
      return *this->textureAtlas;
    }



    return *this->textureAtlas;
  }
  
  std::pair<float, float> FontGPUCache::calculateUV(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    return {x/width, y/height};
  }
}
