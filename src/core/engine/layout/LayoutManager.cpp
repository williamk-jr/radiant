#include "radiant/core/engine/layout/LayoutManager.h"
#include "radiant/core/engine/widgets/widget.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/values/Unit.h"
#include "radiant/util/logger/Logger.h"
#include <memory>
#include <string>

namespace Radiant {
  void LayoutManager::updateLayout(Widget* widget) {
    std::shared_ptr<Widget> parent = widget->getParent();
    std::vector<Widget*> children = widget->getChildren();  

    uint32_t refWidth = parent != nullptr ? parent->getWidth() : 0;
    uint32_t refHeight = parent != nullptr ? parent->getHeight() : 0;

    StyleSheetParser::StyleSheetEntry top = widget->getStyle("top", {{StyleSheetParser::Unit(0, StyleSheetParser::UnitType::PIXEL)}}); 
    StyleSheetParser::StyleSheetEntry bottom = widget->getStyle("bottom", {{StyleSheetParser::Unit(0, StyleSheetParser::UnitType::PIXEL)}}); 
    StyleSheetParser::StyleSheetEntry left = widget->getStyle("left", {{StyleSheetParser::Unit(0, StyleSheetParser::UnitType::PIXEL)}}); 
    StyleSheetParser::StyleSheetEntry right = widget->getStyle("right", {{StyleSheetParser::Unit(0, StyleSheetParser::UnitType::PIXEL)}}); 

    StyleSheetParser::Unit topAbsolute = top.get<StyleSheetParser::ValueTypes::UNIT>(0).value().resolve(refHeight); 
    StyleSheetParser::Unit bottomAbsolute = bottom.get<StyleSheetParser::ValueTypes::UNIT>(0).value().resolve(refHeight); 
    StyleSheetParser::Unit leftAbsolute = left.get<StyleSheetParser::ValueTypes::UNIT>(0).value().resolve(refWidth); 
    StyleSheetParser::Unit rightAbsolute = right.get<StyleSheetParser::ValueTypes::UNIT>(0).value().resolve(refWidth);

    Logger::info("Abs val: " + std::to_string(topAbsolute.getValue()));
    Logger::info("Current Pos: " + std::to_string(widget->getPositionY()));

    widget->setTopOffset(topAbsolute.getValue());
    widget->setBottomOffset(bottomAbsolute.getValue());
    widget->setLeftOffset(leftAbsolute.getValue());
    widget->setRightOffset(rightAbsolute.getValue());
    

    Logger::info("Final pos: " + std::to_string(widget->getPositionY()));

  }
}
