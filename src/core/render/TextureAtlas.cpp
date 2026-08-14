#include "radiant/core/render/TextureAtlas.h"

#include "radiant/util/Box.h"
#include "radiant/util/logger/Logger.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace Radiant {
	TextureAtlas::TextureAtlas(uint32_t width, uint32_t height, uint32_t pixelSize, uint32_t padding)
	    : width(width), height(height), pixelSize(pixelSize), padding(padding) {
		this->buffer = std::vector<uint8_t>();
		this->buffer.reserve(width * height * pixelSize);
	}

	Box TextureAtlas::addTexture(uint8_t* buffer, uint32_t size, uint32_t width, uint32_t height) {
		if (this->cursorX + width >= this->width) {
			if (this->cursorY + this->rowOffset >= this->height) {
				return {-1, -1, -1, -1}; // We cannot go down another line.
			}

			this->cursorX = 0;
			this->cursorY += this->rowOffset + this->padding;
		}

		if (height > this->rowOffset) {
			this->rowOffset = height;
		}

		for (int y = 0; y < height; y++) {
			uint8_t* src = buffer + (width * y * this->pixelSize);
			uint8_t* dst = this->buffer.data() + this->cursorX + (this->width * (this->cursorY + y)) * this->pixelSize;

			std::copy(src, src + (width * this->pixelSize), dst);
		}

		// Get bounds for texture before updating cursor.
		Box uvBounds = this->getUVBoundsAtCursor(width, height);
		this->cursorX += (width + this->padding) * this->pixelSize;
		return uvBounds;
	}

	uint8_t* TextureAtlas::getBuffer() {
		return this->buffer.data();
	}

	uint32_t TextureAtlas::getCursorX() const {
		return this->cursorX;
	}

	uint32_t TextureAtlas::getCursorY() const {
		return this->cursorY;
	}

	uint32_t TextureAtlas::getWidth() const {
		return this->width;
	}

	uint32_t TextureAtlas::getHeight() const {
		return this->height;
	}

	Box TextureAtlas::getUVBoundsAtCursor(uint32_t width, uint32_t height) {
		return {this->cursorX / (float)this->width, this->cursorY / (float)this->height,
		        (this->cursorX + width) / (float)this->width, (this->cursorY + height) / (float)this->height};
	}

	uint32_t TextureAtlas::getPixelSize() const {
		return this->pixelSize;
	}
} // namespace Radiant
