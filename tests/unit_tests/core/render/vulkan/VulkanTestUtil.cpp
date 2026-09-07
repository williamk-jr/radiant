#include "VulkanTestUtil.h"

namespace VulkanTestUtil {
	Radiant::VulkanInstance createTestInstance() {
		std::vector<const char*> instanceLayers     = {"VK_LAYER_KHRONOS_validation"};
		std::vector<const char*> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME,
		                                               VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
		                                               VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

		return Radiant::VulkanInstance("Test Application", instanceExtensions, instanceLayers);
	}

	Radiant::VulkanPhysicalDevice createTestPhysicalDevice(Radiant::VulkanInstance& instance) {
		return Radiant::VulkanPhysicalDevice(instance, [](VkPhysicalDevice& physicalDevice) { return true; });
	}
} // namespace VulkanTestUtil
