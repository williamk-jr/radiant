#pragma once
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"

namespace VulkanTestUtil {
	Radiant::VulkanInstance createTestInstance();

	Radiant::VulkanPhysicalDevice createTestPhysicalDevice(Radiant::VulkanInstance& instance);
} // namespace VulkanTestUtil
