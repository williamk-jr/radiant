#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "radiant/core/engine/LayoutBox.h"
#include "radiant/core/engine/layout/LayoutManager.h"
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

      LayoutManager& getLayoutManager();

      void setPositionX(uint32_t x);
      void setPositionY(uint32_t y);

      void setWidth(uint32_t width);
      void setHeight(uint32_t height);

      void setTopOffset(uint32_t offset);
      void setBottomOffset(uint32_t offset);
      void setLeftOffset(uint32_t offset);
      void setRightOffset(uint32_t offset);

      void updateLayout();
      std::vector<Widget*> getChildren();
      std::shared_ptr<Widget> getParent();
      void addChild(Widget* child);

      void render() override;

    private:
      std::shared_ptr<Widget> parent;
      std::vector<Widget*> children;

      WidgetManager& manager; 
      LayoutManager layoutManager;
      LayoutBox layoutBox;
      std::unique_ptr<StyleSheetParser::StyleSheet> styleSheet;

      Widget(WidgetManager& manager, uint32_t width, uint32_t height);

  };
}
