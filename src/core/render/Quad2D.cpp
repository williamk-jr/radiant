#include "radiant/core/render/Quad2D.h"
#include <vector>

namespace Radiant {
  std::vector<Vertex> Quad2D::getVerticies() {
    return this->verticies;
  }

  std::vector<uint16_t> Quad2D::getIndicies() {
    return this->indicies;
  }
}
