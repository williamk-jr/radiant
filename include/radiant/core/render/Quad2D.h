#pragma once

#include "radiant/core/render/Vertex.h"
#include <cstdint>
#include <vector>
namespace Radiant {
  class Quad2D {
    public:
      std::vector<Vertex> getVerticies();
      std::vector<uint16_t> getIndicies();
    private:
      std::vector<Vertex> verticies{
        //Position  Normal     UV
        {{0, 1, 0}, {0, 0, 1}, {0, 1}}, // Top Left
        {{1, 1, 0}, {0, 0, 1}, {1, 1}}, // Top Right
        {{0, 0, 0}, {0, 0, 1}, {0, 0}}, // Bottom Left
        {{1, 0, 0}, {0, 0, 1}, {1, 0}}  // Bottom Right
      };

      std::vector<uint16_t> indicies{
        0, 2, 3,
        0, 1, 3
      };
  };
}
