
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"

#include "VulkanTestUtil.h"
#include "vulkan/vulkan_core.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>

TEST_CASE("Test Vulkan Physical Device", "[vulkan]") {
	Radiant::VulkanInstance       instance       = VulkanTestUtil::createTestInstance();
	Radiant::VulkanPhysicalDevice physicalDevice = VulkanTestUtil::createTestPhysicalDevice(instance);

	SECTION("Create a physical device.") {
		REQUIRE(physicalDevice.get() != VK_NULL_HANDLE);
	}
}
