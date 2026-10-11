#pragma once

#include <string>
#include <variant>

namespace Radiant::StyleSheetParser {
	enum class StyleSheetProperty {
		WIDTH,
		HEIGHT,
		COLOR,
		BACKGROUND_COLOR,
		MARGIN,
		MARGIN_LEFT,
		MARGIN_RIGHT,
		MARGIN_TOP,
		MARGIN_BOTTOM,
		PADDING,
		PADDING_LEFT,
		PADDING_RIGHT,
		PADDING_TOP,
		PADDING_BOTTOM,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
	};
} // namespace Radiant::StyleSheetParser
