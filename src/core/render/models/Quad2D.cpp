#include "radiant/core/render/models/Quad2D.h"
#include <vector>

namespace Radiant {
  Quad2D::Quad2D() {
    
    this->verticies = {
      //Position   UV
      {{-1,  1}, {0, 1}}, // Top Left
      {{ 1,  1}, {1, 1}}, // Top Right
      {{-1, -1}, {0, 0}}, // Bottom Left
      {{ 1, -1}, {1, 0}}  // Bottom Right
    };

    this->indicies = {
      0, 2, 3,
      0, 1, 3
    };
  }
}
