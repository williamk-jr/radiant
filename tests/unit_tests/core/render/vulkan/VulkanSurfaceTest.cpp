#include "radiant/core/render/vulkan/VulkanSurface.h"

#include "VulkanTestUtil.h"
#include "radiant/core/render/Window.h"
#include "vulkan/vulkan_core.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>
#include <vector>

TEST_CASE("Test Vulkan Surface", "[vulkan]") {
	Radiant::Window window("Testing Window.", 100, 100); // TODO: Option to make window perminantly invisible.
	window.hide();

	Radiant::VulkanInstance instance = VulkanTestUtil::createTestInstance(window);
	Radiant::VulkanSurface  surface  = VulkanTestUtil::createTestSurface(instance, window);

	SECTION("Create a surface.") {
		REQUIRE(surface.get() != VK_NULL_HANDLE);
	}

	SECTION("Creation without instance surface extensions fails.") {
		Radiant::VulkanInstance instance = VulkanTestUtil::createTestInstance();
		REQUIRE_THROWS(VulkanTestUtil::createTestSurface(instance, window));
	}
}
