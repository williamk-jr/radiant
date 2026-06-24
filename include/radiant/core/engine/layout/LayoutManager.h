#pragma once

namespace Radiant {
  class Widget;

  class LayoutManager {
    public:
      LayoutManager() = default;

      void updateLayout(Widget* widget);
      float resolveRelativeStyle();

    private:
  };
}
