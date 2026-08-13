#pragma once

#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/font/cache/FontCache.h"
#include "radiant/core/engine/font/cache/FontGPUCache.h"
#include "radiant/core/render/batch/RenderBatch.h"

#include <filesystem>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <ft2build.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace Radiant {
	class FontManager {
		public:
			FontManager();

			Font                         loadFont(std::filesystem::path path);
			TextureAtlas&                getTextureAtlas();
			bool                         isTextureAtlasDirty();
			void                         markTextureAtlasClean();
			std::unique_ptr<RenderBatch> compileStringGeometry(Font& font, std::string str, uint32_t x, uint32_t y);

		private:
			std::unique_ptr<FontCache>    fontCache;
			std::unique_ptr<FontGPUCache> fontGpuCache;
			FT_BitmapGlyph                toBitmapGlyph(FT_Glyph glyph, FT_Render_Mode renderMode);
	};
} // namespace Radiant
