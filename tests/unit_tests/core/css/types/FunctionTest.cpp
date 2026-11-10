#include "radiant/css/types/Function.h"

#include "radiant/css/StyleSheetValue.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>

using namespace Radiant::StyleSheetParser;

TEST_CASE("Test Function type.", "[CSS_Parser]") {
	Types::Function func{
	    "test", [](std::vector<StyleSheetValue> params) { return StyleSheetValue{Types::Integer(10)}; }, {}};

	SECTION("Test percentage unit resolution.") {
		func.resolve(0.0f);
		// Radiant::StyleSheetParser::Unit resolvedInit = unit.resolve(100);
		// REQUIRE(resolvedInit.getValue() == 20.0);
	}
}
