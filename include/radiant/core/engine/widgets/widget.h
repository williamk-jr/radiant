#pragma once
#include <cstdint>
#include <memory>
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

      Widget(std::shared_ptr<Widget> parent, Unit width, Unit height);
      Widget(std::shared_ptr<Widget> parent, uint32_t positionX, uint32_t positionY, Unit width, Unit height);

      uint32_t getPositionX();
      uint32_t getPositionY();
      
      uint32_t getWidth();
      uint32_t getHeight();

      void updateLayout();
      std::vector<Widget*> getChildren();
      void addChild(Widget* child);

      void render() override;

    private:
      std::shared_ptr<Widget> parent;
      std::vector<Widget*> children;
      PositionType positionType = PositionType::RELATIVE;

      uint32_t positionX;
      uint32_t positionY;

      Unit width;
      Unit height;
  };
}
