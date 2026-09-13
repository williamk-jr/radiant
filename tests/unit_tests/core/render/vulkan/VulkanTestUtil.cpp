#include "VulkanTestUtil.h"

#include "radiant/core/render/Window.h"
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"

namespace VulkanTestUtil {
	Radiant::VulkanInstance createTestInstance() {
		std::vector<const char*> instanceLayers     = {"VK_LAYER_KHRONOS_validation"};
		std::vector<const char*> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME,
		                                               VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
		                                               VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

		return Radiant::VulkanInstance("Test Application", instanceExtensions, instanceLayers);
	}

	Radiant::VulkanInstance createTestInstance(Radiant::Window& window) {
		std::vector<const char*> instanceLayers     = {"VK_LAYER_KHRONOS_validation"};
		std::vector<const char*> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME,
		                                               VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
		                                               VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

		for (const char* surfaceExtension : window.getSurfaceExtensions()) {
			instanceExtensions.push_back(surfaceExtension);
		}

		return Radiant::VulkanInstance("Test Application", instanceExtensions, instanceLayers);
	}

	Radiant::VulkanPhysicalDevice createTestPhysicalDevice(Radiant::VulkanInstance& instance) {
		return Radiant::VulkanPhysicalDevice(instance, [](VkPhysicalDevice& physicalDevice) { return true; });
	}

	Radiant::VulkanSurface createTestSurface(Radiant::VulkanInstance& instance, Radiant::Window& window) {
		return Radiant::VulkanSurface(instance, window.getHandle());
	}
} // namespace VulkanTestUtil
