
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"

#include "VulkanTestUtil.h"
#include "vulkan/vulkan_core.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>
#include <vector>

TEST_CASE("Test Vulkan Physical Device", "[vulkan]") {
	Radiant::VulkanInstance       instance       = VulkanTestUtil::createTestInstance();
	Radiant::VulkanPhysicalDevice physicalDevice = VulkanTestUtil::createTestPhysicalDevice(instance);

	SECTION("Create a physical device.") {
		REQUIRE(physicalDevice.get() != VK_NULL_HANDLE);
	}

	SECTION("Get physical device properties.") {
		VkPhysicalDeviceProperties2 deviceProperties = physicalDevice.getProperties();
		REQUIRE(deviceProperties.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2);
	}

	SECTION("Get queue family properties.") {
		std::vector<VkQueueFamilyProperties2> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
		REQUIRE(queueFamilyProperties.size() != 0);

		for (VkQueueFamilyProperties2 properties : queueFamilyProperties) {
			REQUIRE(properties.sType == VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2);
		}
	}
}
