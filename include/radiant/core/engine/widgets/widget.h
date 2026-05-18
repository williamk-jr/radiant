#pragma once
#include <vector>
#include "radiant/core/render/renderable.h"

namespace Radiant {
  class Widget: public Renderable {
    public:

      Widget(Widget& parent, int width, int height);
      Widget(Widget& parent, int positionX, int positionY, int width, int height);

      int getPositionX();
      int getPositionY();
      
      int getWidth();
      int getHeight();

      void render() override;

    private:
      Widget& parent;
      std::vector<Widget> children;

      int positionX;
      int positionY;

      int width;
      int height;
  };
}
