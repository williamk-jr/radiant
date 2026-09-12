#include <catch2/catch_test_macros.hpp>
#include <radiant/core/engine/LayoutBox.h>

TEST_CASE("Test LayoutBox", "[layout]") {
	Radiant::LayoutBox layoutBox{};

	SECTION("Test initial state.") {
		REQUIRE(layoutBox.getContentBoxWidth() == 0);
		REQUIRE(layoutBox.getContentBoxHeight() == 0);
		REQUIRE(layoutBox.getContentBoxX() == 0);
		REQUIRE(layoutBox.getContentBoxY() == 0);
	}

	SECTION("") {
		// REQUIRE();
	}
}
