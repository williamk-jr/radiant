#pragma once

#include "radiant/core/engine/font/cache/FontCacheIdentifier.h"
#include "radiant/core/render/TextureAtlas.h"
#include "radiant/util/Box.h"

#include <cstddef>
#include <cstdint>
#include <freetype/ftglyph.h>
#include <freetype/ftimage.h>
#include <freetype/fttypes.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace Radiant {
	struct GlyphIdentifier {
			FontCacheIdentifier fontId;
			unsigned long       charCode;
			uint32_t            size;

			bool operator==(const GlyphIdentifier& other) const {
				return this->fontId == other.fontId && this->charCode == other.charCode && this->size == other.size;
			}
	};

	struct GlyphIdentifierHasher {
			std::size_t operator()(const GlyphIdentifier& id) const noexcept {
				std::size_t hash = FontCacheIdentifierHasher{}(id.fontId);
				hash_util::combineHash<uint32_t>(hash, id.charCode);
				hash_util::combineHash<uint32_t>(hash, id.size);
				return hash;
			}
	};

	struct GlyphEntry {
			Box       uv;
			size_t    width;
			size_t    height;
			FT_Vector advance;
			FT_Int    top;
			FT_Int    left;
	};

	/*
	 * Cache glyphs for use in gpu texture atlas generation.
	 *
	 * Glyphs will be cached here as they are found by the font manager.
	 * The cached glyphs can then be stiched into a texture atlas.
	 *
	 * If the cache state has been changed (eg. a new glyph was added),
	 * the texture atlas will be rebuilt to match its new state.
	 *
	 * If the texture atlas cannot simply be recreated with new state appended,
	 * such as when the atlas must be repacked to fit memory constraints,
	 * the UV coordinates of every glyph must be updated to match the shifted positions.
	 */
	class FontGPUCache {
		public:
			FontGPUCache();

			void addEntry(FT_BitmapGlyph& bitmapGlyph, FT_Vector advance, GlyphIdentifier identifier);

			GlyphEntry getEntry(GlyphIdentifier identifier);

			bool hasEntry(GlyphIdentifier identifier);

			bool isDirty();

			void markClean();

			TextureAtlas& getTextureAtlas();

		private:
			std::unordered_map<GlyphIdentifier, GlyphEntry, GlyphIdentifierHasher> cache;
			std::unique_ptr<TextureAtlas>                                          textureAtlas;
			bool                                                                   cacheDirty;
	};
} // namespace Radiant
