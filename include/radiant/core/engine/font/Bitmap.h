#pragma once

#include <vector>

namespace Radiant {
  class FontManager;
  
  class Bitmap {
    public:
      Bitmap(unsigned char* buffer, size_t size, size_t width, size_t height);
    private:
      std::vector<unsigned char> buffer;
      size_t width;
      size_t height;
  };
}
 
