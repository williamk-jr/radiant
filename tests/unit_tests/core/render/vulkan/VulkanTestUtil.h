#pragma once
#include "radiant/core/render/Window.h"
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"

namespace VulkanTestUtil {
	Radiant::VulkanInstance createTestInstance();
	Radiant::VulkanInstance createTestInstance(Radiant::Window& window);

	Radiant::VulkanPhysicalDevice createTestPhysicalDevice(Radiant::VulkanInstance& instance);
	Radiant::VulkanSurface        createTestSurface(Radiant::VulkanInstance& instance, Radiant::Window& window);
} // namespace VulkanTestUtil
