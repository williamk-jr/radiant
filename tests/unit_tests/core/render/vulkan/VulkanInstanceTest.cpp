#include "VulkanTestUtil.h"
#include "vulkan/vulkan_core.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>

TEST_CASE("Test Vulkan Instance", "[vulkan]") {
	Radiant::VulkanInstance instance = VulkanTestUtil::createTestInstance();

	SECTION("Create an instance.") {
		REQUIRE(instance.get() != VK_NULL_HANDLE);
	}

	SECTION("Create an instance with no extensions or layers.") {
		instance = Radiant::VulkanInstance("Test Application");
		REQUIRE(instance.get() != VK_NULL_HANDLE);
	}
}
