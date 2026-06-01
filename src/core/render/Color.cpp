#include "radiant/core/render/Color.h"

namespace Radiant {
  Color::Color(float r, float g, float b, float a) {
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    this->color[3] = a;
  }

  float* Color::raw() {
    return this->color;
  }
}
