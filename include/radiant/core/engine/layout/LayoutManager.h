#pragma once

#include "radiant/core/engine/widgets/widget.h"
namespace Radiant {
  class LayoutManager {
    public:
      LayoutManager(Widget* widget);

      void updateLayout();
      float resolveRelativeStyle();

    private:
  };
}
