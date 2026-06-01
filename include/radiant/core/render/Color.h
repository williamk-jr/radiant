#pragma once

namespace Radiant {
  class Color {
    public:
      Color(float r, float g, float b, float a);

      float* raw();
    private:
      float color[4];
  };
}
