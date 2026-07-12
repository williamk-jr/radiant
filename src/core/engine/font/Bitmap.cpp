#include "radiant/core/engine/font/Bitmap.h"
#include "radiant/util/logger/Logger.h"
#include <cstring>
#include <string>


namespace Radiant {
  Bitmap::Bitmap(unsigned char* buffer, size_t size, size_t width, size_t height) {
    Logger::info(std::to_string(size));
    Logger::info(std::to_string(width));
    Logger::info(std::to_string(height));
    this->buffer.reserve(size);
    memcpy(this->buffer.data(), buffer, size);
  }
}
