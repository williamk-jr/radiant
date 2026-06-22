#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "radiant/core/engine/LayoutBox.h"
#include "radiant/core/render/renderable.h"
#include "radiant/css/Parser.h"

namespace Radiant {
  class WidgetManager;

  enum class PositionType {
    ABSOLUTE,
    RELATIVE,
    FIXED
  };

  class Widget: public Renderable {
    friend WidgetManager;
    public:

      Widget(std::shared_ptr<Widget> parent, uint32_t width, uint32_t height);
      Widget(std::shared_ptr<Widget> parent, uint32_t positionX, uint32_t positionY, uint32_t width, uint32_t height);

      void addStyle(std::string name, StyleSheetParser::StyleSheetEntry entry);
      StyleSheetParser::StyleSheetEntry getStyle(std::string name, StyleSheetParser::StyleSheetEntry defaultEntry);

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

      WidgetManager& manager; 
      LayoutBox layoutBox;
      StyleSheetParser::StyleSheet styleSheet;

      Widget(WidgetManager& manager, uint32_t width, uint32_t height);

  };
}
