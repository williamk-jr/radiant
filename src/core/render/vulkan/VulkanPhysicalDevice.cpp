#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"

#include "radiant/core/render/vulkan/VulkanResult.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"

#include <vulkan/vulkan_core.h>

namespace Radiant {
	VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance&                  instance,
	                                           VulkanPhysicalDeviceRequirements physicalDeviceRequirements) {
		VkResult result              = VK_SUCCESS;
		uint32_t physicalDeviceCount = 0;
		result                       = vkEnumeratePhysicalDevices(instance.get(), &physicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		result = vkEnumeratePhysicalDevices(instance.get(), &physicalDeviceCount, physicalDevices.data());
		VulkanUtil::validate("VulkanPhysicalDevice Initialization Error. Failed to enumerate physical devices.",
		                     result);

		for (VkPhysicalDevice& physicalDevice : physicalDevices) {
			if (physicalDeviceRequirements(physicalDevice)) {
				this->physicalDevice = physicalDevice;
				return;
			}
		}

		Logger::fatal(
		    "VulkanPhysicalDevice Initialization Error. Failed to find a device that meets the provided requirements.");
	}

	VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanPhysicalDevice&& other) noexcept
	    : physicalDevice(other.physicalDevice) {
		other.physicalDevice = nullptr;
	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice() {}

	VkPhysicalDevice VulkanPhysicalDevice::get() {
		return this->physicalDevice;
	}

	VkPhysicalDeviceProperties2 VulkanPhysicalDevice::getProperties() {
		VkPhysicalDeviceProperties2 properties = {};
		vkGetPhysicalDeviceProperties2(this->physicalDevice, &properties);
		return properties;
	}

	std::vector<VkQueueFamilyProperties2> VulkanPhysicalDevice::getQueueFamilyProperties() {
		uint32_t queuePropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties2(this->physicalDevice, &queuePropertyCount, nullptr);

		std::vector<VkQueueFamilyProperties2> queueFamilyProperties(queuePropertyCount,
		                                                            {VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2});
		vkGetPhysicalDeviceQueueFamilyProperties2(this->physicalDevice, &queuePropertyCount,
		                                          queueFamilyProperties.data());
		return queueFamilyProperties;
	}

	VulkanResult<std::vector<VkSurfaceFormat2KHR>> VulkanPhysicalDevice::getSurfaceFormats(VulkanSurface& surface) {
		VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
		surfaceInfo.sType   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
		surfaceInfo.surface = surface.get();

		uint32_t surfaceFormatCount = 0;
		VkResult result =
		    vkGetPhysicalDeviceSurfaceFormats2KHR(this->physicalDevice, &surfaceInfo, &surfaceFormatCount, nullptr);
		std::vector<VkSurfaceFormat2KHR> surfaceFormats(surfaceFormatCount, {VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR});
		result = vkGetPhysicalDeviceSurfaceFormats2KHR(this->physicalDevice, &surfaceInfo, &surfaceFormatCount,
		                                               surfaceFormats.data());
		return {result, surfaceFormats};
	}

	VulkanResult<bool> VulkanPhysicalDevice::queueFamilySupportsSurfaceKHR(VulkanSurface& surface,
	                                                                       uint32_t       queueFamily) {
		VkBool32 supportsSurface = false;
		VkResult result =
		    vkGetPhysicalDeviceSurfaceSupportKHR(this->physicalDevice, queueFamily, surface.get(), &supportsSurface);
		return {result, (bool)supportsSurface};
	}
} // namespace Radiant
