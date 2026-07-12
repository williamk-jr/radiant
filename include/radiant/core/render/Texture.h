#pragma once
#include <cstddef>
#include <filesystem>
#include <vector>

namespace Radiant {
  class Texture {
    public:
      static Texture& fromPath(std::filesystem::path path);
      static Texture& fromMemory(void* buffer);

    private:
      void* buffer;
  };
}
