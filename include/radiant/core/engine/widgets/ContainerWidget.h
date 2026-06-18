#pragma once

#include "radiant/core/engine/layout/LayoutManager.h"
#include "radiant/core/engine/widgets/widget.h"
namespace Radiant {
  class ContainerWidget : public Widget {
    public:
      ContainerWidget();

      void setLayoutManager();
    private:
      std::unique_ptr<LayoutManager> layoutManager;
  };
}
