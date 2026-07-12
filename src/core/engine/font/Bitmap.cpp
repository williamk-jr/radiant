#include "radiant/core/engine/font/Bitmap.h"
#include "radiant/util/logger/Logger.h"
#include <cstring>
#include <string>


namespace Radiant {
  Bitmap::Bitmap(unsigned char* buffer, size_t size, size_t width, size_t height) {
    this->buffer.reserve(size);

    for (int i = 0; i < height; i++) {
      unsigned char* srcRow = buffer + i * (size/height); 
      unsigned char* dstRow = this->buffer.data() + i * width; 
      memcpy(dstRow, srcRow, width);
    }

  }
}
