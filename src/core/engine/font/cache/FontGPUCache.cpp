#include "radiant/core/engine/font/cache/FontGPUCache.h"

#include "radiant/core/render/TextureAtlas.h"
#include "radiant/util/Box.h"
#include "radiant/util/debug/ExecutionProfiler.h"
#include "radiant/util/logger/Logger.h"

#include <freetype/ftglyph.h>
#include <freetype/ftimage.h>
#include <memory>
#include <string>
#include <utility>

namespace Radiant {
	FontGPUCache::FontGPUCache() {
		this->textureAtlas = std::make_unique<TextureAtlas>(2048, 2048, 1, 1);
	}

	void FontGPUCache::addEntry(FT_BitmapGlyph& bitmapGlyph, FT_Vector advance, GlyphIdentifier identifier) {
		FT_Bitmap bitmap = bitmapGlyph->bitmap;

		size_t bufferSize = bitmap.rows * bitmap.pitch;
		size_t width      = bitmap.width;
		size_t height     = bitmap.rows;

		Box uvBounds = this->textureAtlas->addTexture(bitmap.buffer, bufferSize, width, height);

		// Logger::info("Char: "+std::to_string((char)identifier.charCode)+
		//               "\nMin: ("+std::to_string(uvBounds.minX)+",
		//               "+std::to_string(uvBounds.minY)+
		//               "\nMax: ("+std::to_string(uvBounds.maxX)+",
		//               "+std::to_string(uvBounds.maxY) +"\n");

		this->cache[identifier] = {uvBounds, width, height, advance, bitmapGlyph->top, bitmapGlyph->left};
		this->cacheDirty        = true;
	}

	GlyphEntry FontGPUCache::getEntry(GlyphIdentifier identifier) {
		return this->cache[identifier];
	}

	bool FontGPUCache::hasEntry(GlyphIdentifier identifier) {
		return this->cache.find(identifier) != this->cache.end();
	}

	bool FontGPUCache::isDirty() {
		return this->cacheDirty;
	}

	void FontGPUCache::markClean() {
		this->cacheDirty = false;
	}

	TextureAtlas& FontGPUCache::getTextureAtlas() {
		return *this->textureAtlas;
	}
} // namespace Radiant
