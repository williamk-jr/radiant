#include "radiant/core/engine/RadiantEngine.h"
#include "radiant/core/engine/widgets/widget.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/values/Color.h"
#include "radiant/css/values/Unit.h"
#include "radiant/util/logger/Logger.h"

#include <memory>
#include <string>
#include <vector>

/* TODO
 *
 * ----CSS Parser----
 * - Integrate CSS parser with widget styles.
 * - Implement CSS selectors.
 *   - Turn parsed selectors into a lookup function.
 *   - Integrate lookup function into widget identifier system.
 * - Implement @ properties.
 *
 * ----Layout Manager----
 * - Integrate more stylesheet properties.
 * - Add grid and/or flexbox layouts.
 *
 * ----Widgets----
 * - Implement text rendering.
 * - Implement transparency.
 *   - Include gamma-correct color blending.
 *   - Read paper on weight-blended OIT.
 * - Implement border styles.
 */

int main() {
	Radiant::Logger::shouldAllowColor(true);
	Radiant::Logger::shouldAllowExceptions(false);
	Radiant::Logger::setVerbosity(1);

	const std::string      applicationName = "Test";
	Radiant::RadiantEngine engine(applicationName, 500, 500);

	std::vector<std::shared_ptr<Radiant::Widget>> widgets;

	for (int i = 0; i < 6; i++) {
		widgets.push_back(std::make_shared<Radiant::Widget>(engine.getRootWidget(), 0, 0, 200, 100));

		widgets[i]->addStyle("left",
		                     {{Radiant::StyleSheetParser::Unit(10.0, Radiant::StyleSheetParser::UnitType::PIXEL)}});

		widgets[i]->addStyle("background-color", {{Radiant::StyleSheetParser::Color(0, 255, 0, 127)}});
	}
	// testWidget.addStyle("left", {
	//     {Radiant::StyleSheetParser::Unit(10.0,
	//     Radiant::StyleSheetParser::UnitType::PIXEL)}
	// });
	// testWidget.addStyle("top", {
	//     {Radiant::StyleSheetParser::Unit(10.0,
	//     Radiant::StyleSheetParser::UnitType::PERCENTAGE)}
	// });

	while (engine.isRunning()) {
		engine.update();
	}
}
