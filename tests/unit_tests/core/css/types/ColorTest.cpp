#include "radiant/css/types/Color.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>

using namespace Radiant::StyleSheetParser;

TEST_CASE("Test Color type.", "[CSS_Parser]") {
	Types::Color color{16, 35, 115, 243};

	SECTION("Test Hex/Single Integer Initialization") {
		color = {0xFF00FFFF};
		REQUIRE(color.getRed() == 255);
		REQUIRE(color.getGreen() == 0);
		REQUIRE(color.getBlue() == 255);
		REQUIRE(color.getAlpha() == 255);
	}

	SECTION("Test Keyword Initialization") {
		color = {Types::Colors::GREEN};
		REQUIRE(color.getRed() == 0);
		REQUIRE(color.getGreen() == 255);
		REQUIRE(color.getBlue() == 0);
		REQUIRE(color.getAlpha() == 255);
	}

	SECTION("Test getRed()") {
		REQUIRE(color.getRed() == 16);
	}

	SECTION("Test getGreen()") {
		REQUIRE(color.getGreen() == 35);
	}

	SECTION("Test getRed()") {
		REQUIRE(color.getBlue() == 115);
	}

	SECTION("Test getRed()") {
		REQUIRE(color.getAlpha() == 243);
	}
}
