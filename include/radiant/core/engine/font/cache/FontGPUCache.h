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

	/**
	 * An identifier for glyphs in the atlas cache.
	 */
	struct GlyphIdentifier {
			/**
			 * An identifier for the font that the glyph belongs to.
			 */
			FontCacheIdentifier fontId;

			/**
			 * The glyph's character code.
			 */
			unsigned long charCode;

			/**
			 * The font size in pixels.
			 */
			uint32_t size;

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

	/**
	 * An entry containing the texture atlas position and typographic information of a glyph.
	 */
	struct GlyphEntry {
			Box       uv;
			size_t    width;
			size_t    height;
			FT_Vector advance;
			FT_Int    top;
			FT_Int    left;
	};

	/**
	 * Caches glyphs in a texture atlas. Each time a glyph is added, the cache is marked as dirty.
	 * Applications are expected to decide when to mark the cache as clean.
	 */
	class FontGPUCache {
		public:
			FontGPUCache();

			/**
			 * Adds a bitmap to the cache.
			 *
			 * @param FT_BitmapGlyph A reference to the bitmap.
			 * @param FT_Vector How far to advance foward after the bitmap.
			 * @param GlyphIdentifier A unique identifier for the provided glyph.
			 */
			void addEntry(FT_BitmapGlyph& bitmapGlyph, FT_Vector advance, GlyphIdentifier identifier);

			/**
			 * Retrieves an entry from the cache.
			 *
			 * @param GlyphIdentifier A unique identifier for the cached glyph.
			 * @return A glyph entry.
			 */
			GlyphEntry getEntry(GlyphIdentifier identifier);

			/**
			 * Checks if an entry is in the cache.
			 *
			 * @param GlyphIdentifier A unique identifier for the cached glyph.
			 * @return Whether the glyph is in the cache.
			 */
			bool hasEntry(GlyphIdentifier identifier);

			/**
			 * Checks if the cache has been modified.
			 * Gives the application control over when to mark this cache as unmodified, as each application may use
			 * this cache differently.
			 *
			 * @return Whether the cache has been modified.
			 */
			bool isDirty();

			/**
			 * Set the cache as unmodified.
			 * Gives the application control over when to mark this cache as unmodified, as each application may use
			 * this cache differently.
			 */
			void markClean();

			/**
			 * Retrieves the current texture atlas.
			 * @return The texture atlas.
			 */
			TextureAtlas& getTextureAtlas();

		private:
			std::unordered_map<GlyphIdentifier, GlyphEntry, GlyphIdentifierHasher> cache;
			std::unique_ptr<TextureAtlas>                                          textureAtlas;
			bool                                                                   cacheDirty;
	};
} // namespace Radiant
