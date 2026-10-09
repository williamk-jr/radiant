#include "radiant/core/render/vulkan/VulkanDevice.h"

#include "VulkanTestUtil.h"
#include "radiant/core/render/WindowSystem.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "vulkan/vulkan_core.h"

#include <catch2/catch_test_macros.hpp>
#include <radiant/core/render/vulkan/VulkanInstance.h>
#include <vector>

TEST_CASE("Test Vulkan Device", "[vulkan]") {
	Radiant::WindowSystem windowSystem{};
	Radiant::Window       window = Radiant::Window::createDummy(100, 100);

	Radiant::VulkanInstance       instance       = VulkanTestUtil::createTestInstance(window);
	Radiant::VulkanPhysicalDevice physicalDevice = VulkanTestUtil::createTestPhysicalDevice(instance);
	Radiant::VulkanSurface        surface        = VulkanTestUtil::createTestSurface(instance, window);
	Radiant::VulkanDevice         device         = VulkanTestUtil::createTestDevice(physicalDevice, surface);

	SECTION("Create a logical device.") {
		REQUIRE(device.get() != VK_NULL_HANDLE);
	}

	SECTION("Get graphics queue family.") {
		REQUIRE(device.getGraphicsQueueFamily() >= 0);
	}

	SECTION("Get present queue family.") {
		REQUIRE(device.getPresentQueueFamily() >= 0);
	}
}
