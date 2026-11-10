#include "radiant/css/types/Unit.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>

TEST_CASE("Test unit type.", "[CSS_Parser]") {
	Radiant::StyleSheetParser::Types::Unit unit{20, Radiant::StyleSheetParser::Types::UnitType::PERCENTAGE};

	SECTION("Test percentage unit resolution.") {
		Radiant::StyleSheetParser::Types::Unit resolvedInit = unit.resolve(100);
		REQUIRE(resolvedInit.getValue() == 20.0);
	}
}
