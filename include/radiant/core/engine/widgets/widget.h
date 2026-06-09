#pragma once
#include <cstdint>
#include <vector>
#include "radiant/core/engine/widgets/Unit.h"
#include "radiant/core/render/renderable.h"

namespace Radiant {
  enum class PositionType {
    ABSOLUTE,
    RELATIVE,
    FIXED
  };

  class Widget: public Renderable {
    public:

      Widget(Widget* parent, Unit width, Unit height);
      Widget(Widget* parent, uint32_t positionX, uint32_t positionY, Unit width, Unit height);

      uint32_t getPositionX();
      uint32_t getPositionY();
      
      uint32_t getWidth();
      uint32_t getHeight();

      void updateLayout();

      void render() override;

    private:
      Widget* parent;
      std::vector<Widget*> children;
      PositionType positionType = PositionType::RELATIVE;

      uint32_t positionX;
      uint32_t positionY;

      Unit width;
      Unit height;
  };
}
