#include "radiant/core/engine/layout/LayoutManager.h"

#include "radiant/core/engine/widgets/widget.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/types/Unit.h"
#include "radiant/util/logger/Logger.h"

#include <memory>
#include <string>

/*
 * Finalize current position
 *
 * Prevent overlap of children
 *
 * largestHeight = 0
 *
 * offsetX = 0
 * offsetY = 0
 *
 * for child in children:
 *    child.setPosition(offsetX, offsetY)
 *
 *    if ( (child.getX() + child.getWidth()) > (current.getX() + current.getWidth()) ):
 *      offsetX = 0;
 *      offsetY += largestHeight;
 *      largestHeight = 0;
 *    else:
 *      offsetX += child.getWidth()
 *      if (child.getHeight() > largestHeight):
 *        largestHeight = child.getHeight()
 *
 *
 *
 */

namespace Radiant {
	void LayoutManager::updateLayout(Widget* widget) {
		std::shared_ptr<Widget> parent   = widget->getParent();
		std::vector<Widget*>    children = widget->getChildren();

		uint32_t refWidth  = parent != nullptr ? parent->getWidth() : 0;
		uint32_t refHeight = parent != nullptr ? parent->getHeight() : 0;

		StyleSheetParser::StyleSheetEntry top =
		    widget->getStyle(StyleSheetParser::StyleSheetProperty::TOP,
		                     {{StyleSheetParser::Types::Unit(0, StyleSheetParser::Types::UnitType::PIXEL)}});
		StyleSheetParser::StyleSheetEntry bottom =
		    widget->getStyle(StyleSheetParser::StyleSheetProperty::BOTTOM,
		                     {{StyleSheetParser::Types::Unit(0, StyleSheetParser::Types::UnitType::PIXEL)}});
		StyleSheetParser::StyleSheetEntry left =
		    widget->getStyle(StyleSheetParser::StyleSheetProperty::LEFT,
		                     {{StyleSheetParser::Types::Unit(0, StyleSheetParser::Types::UnitType::PIXEL)}});
		StyleSheetParser::StyleSheetEntry right =
		    widget->getStyle(StyleSheetParser::StyleSheetProperty::RIGHT,
		                     {{StyleSheetParser::Types::Unit(0, StyleSheetParser::Types::UnitType::PIXEL)}});

		StyleSheetParser::Types::Unit topAbsolute =
		    top.get<StyleSheetParser::Types::Unit>(0).value().resolve(refHeight);
		StyleSheetParser::Types::Unit bottomAbsolute =
		    bottom.get<StyleSheetParser::Types::Unit>(0).value().resolve(refHeight);
		StyleSheetParser::Types::Unit leftAbsolute =
		    left.get<StyleSheetParser::Types::Unit>(0).value().resolve(refWidth);
		StyleSheetParser::Types::Unit rightAbsolute =
		    right.get<StyleSheetParser::Types::Unit>(0).value().resolve(refWidth);

		widget->setTopOffset(topAbsolute.getValue());
		widget->setBottomOffset(bottomAbsolute.getValue());
		widget->setLeftOffset(leftAbsolute.getValue());
		widget->setRightOffset(rightAbsolute.getValue());

		/* Child Layout Manager.
		 *
		 * All children are laid out in relation to the parent's content box.
		 * Children are position using their margin box, as it is the outermost layer in the box model.
		 */

		const LayoutBox& widgetLayoutBox = widget->getLayoutBox();

		uint32_t offsetX       = 0;
		uint32_t offsetY       = 0;
		uint32_t largestHeight = 0;

		for (Widget* child : children) {
			const LayoutBox& childLayoutBox = child->getLayoutBox();

			// Prevent overflow, wrap to next line
			if ((offsetX + childLayoutBox.getMarginBoxWidth())
			    > (widgetLayoutBox.getContentBoxX() + widgetLayoutBox.getContentBoxWidth())) {
				offsetY += largestHeight;
				offsetX       = 0;
				largestHeight = 0;
			}

			// Update child
			child->setPositionX(widgetLayoutBox.getContentBoxX() + offsetX);
			child->setPositionY(widgetLayoutBox.getContentBoxY() + offsetY);
			child->updateLayout();

			// Update offset
			offsetX += childLayoutBox.getMarginBoxWidth();
			if (childLayoutBox.getMarginBoxHeight() > largestHeight) {
				largestHeight = childLayoutBox.getMarginBoxHeight();
			}
		}
	}
} // namespace Radiant
