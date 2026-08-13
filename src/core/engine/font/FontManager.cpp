#include "radiant/core/engine/font/FontManager.h"

#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/font/cache/FontCacheNode.h"
#include "radiant/core/engine/font/cache/FontGPUCache.h"
#include "radiant/core/render/TextureAtlas.h"
#include "radiant/core/render/batch/RenderBatch.h"
#include "radiant/core/render/models/Quad2D.h"
#include "radiant/util/debug/ExecutionProfiler.h"
#include "radiant/util/logger/Logger.h"

#include <cstdlib>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/fttypes.h>
#include <memory>
#include <string>

namespace Radiant {
	FontManager::FontManager() {
		this->fontCache    = std::make_unique<FontCache>(1024 * 4, FONT_CACHE_GLYPH | FONT_CACHE_SMALL_BITMAP);
		this->fontGpuCache = std::make_unique<FontGPUCache>();
	}

	Font FontManager::loadFont(std::filesystem::path path) {
		return {*this->fontCache, {path, 0}};
	}

	std::unique_ptr<RenderBatch>
	FontManager::compileStringGeometry(Font& font, std::string str, uint32_t x, uint32_t y) {
		// Debug::ExecutionProfiler profiler{"compileStringGeometry", true};
		// profiler.begin();

		uint32_t                     offsetX     = 0;
		uint32_t                     offsetY     = 0;
		std::unique_ptr<RenderBatch> renderBatch = std::make_unique<RenderBatch>();
		renderBatch->instances.reserve(str.size());

		for (char charCode : str) {
			//  Check if glyph in cache

			// Check if glyph in gpu cache.
			GlyphIdentifier glyphId = {font.fontFaceIdentifier, (unsigned long)charCode, font.size};
			if (!fontGpuCache->hasEntry(glyphId)) {

				FontCacheNode<FT_Glyph> glyphNode =
				    this->fontCache->lookupGlyph(font.fontFaceIdentifier, charCode, font.size, font.size);
				if (glyphNode.isEmpty()) {
					Logger::info("Could not load char: " + std::to_string(charCode));
					continue;
				}

				FT_BitmapGlyph bitmapGlyph = this->toBitmapGlyph(glyphNode.getValue(), FT_RENDER_MODE_NORMAL);
				fontGpuCache->addEntry(bitmapGlyph, glyphNode.getValue()->advance, glyphId);
			}

			GlyphEntry glyphEntry = this->fontGpuCache->getEntry(glyphId);
			// Logger::info(std::to_string(x));
			renderBatch->instances.emplace_back(Instance{{0, 0, 0, 255},
			                                             {x + offsetX, y + offsetY},
			                                             {glyphEntry.width, glyphEntry.height},
			                                             {glyphEntry.uv.minX, glyphEntry.uv.minY},
			                                             {glyphEntry.uv.maxX, glyphEntry.uv.maxY}});

			offsetX += glyphEntry.advance.x >> 16;
			offsetY += glyphEntry.advance.y >> 16;
		}

		// profiler.end();
		return std::move(renderBatch);
	}

	TextureAtlas& FontManager::getTextureAtlas() {
		return this->fontGpuCache->getTextureAtlas();
	}

	bool FontManager::isTextureAtlasDirty() {
		return this->fontGpuCache->isDirty();
	}

	void FontManager::markTextureAtlasClean() {
		this->fontGpuCache->markClean();
	}

	FT_BitmapGlyph FontManager::toBitmapGlyph(FT_Glyph glyph, FT_Render_Mode renderMode) {
		if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
			FT_Glyph_To_Bitmap(&glyph, renderMode, nullptr, false);
		}
		return (FT_BitmapGlyph)glyph;
	}
} // namespace Radiant
