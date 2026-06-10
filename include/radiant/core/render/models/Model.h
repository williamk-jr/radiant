#pragma once
#include "radiant/core/render/Vertex.h"
#include <vector>

namespace Radiant {
  class Model {
    public:
      std::vector<Vertex> getVerticies();
      std::vector<uint16_t> getIndicies();
    protected:
      std::vector<Vertex> verticies;
      std::vector<uint16_t> indicies;
  };
}
