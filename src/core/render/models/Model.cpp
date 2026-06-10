#include "radiant/core/render/models/Model.h"
#include <vector>

namespace Radiant {
  std::vector<Vertex> Model::getVerticies() {
    return this->verticies;
  }

  std::vector<uint16_t> Model::getIndicies() {
    return this->indicies;
  }
}
