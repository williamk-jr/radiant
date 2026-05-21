#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include <vulkan/vulkan_core.h>

namespace Radiant { 
  VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance& instance, VulkanPhysicalDeviceRequirements physicalDeviceRequirements) {
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(instance.get(), &physicalDeviceCount, nullptr);

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance.get(), &physicalDeviceCount, physicalDevices.data());
    
    for (VkPhysicalDevice& physicalDevice : physicalDevices) {
      if (physicalDeviceRequirements(physicalDevice)) {
        this->physicalDevice = physicalDevice;
        return;
      }
    }

    Logger::fatal("Unable to find a device that meets the provided requirements.");
  }

  VulkanPhysicalDevice::~VulkanPhysicalDevice() {
    
  }

  VkPhysicalDevice& VulkanPhysicalDevice::get() {
    return this->physicalDevice;
  }

  void VulkanPhysicalDevice::getProperties(VkPhysicalDeviceProperties2* properties) {
    vkGetPhysicalDeviceProperties2(this->physicalDevice, properties);
  }
  
  std::vector<VkQueueFamilyProperties2> VulkanPhysicalDevice::getQueueFamilyProperties() {
    uint32_t queuePropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties2(this->physicalDevice, &queuePropertyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties2> queueFamilyProperties(queuePropertyCount, {VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2});
    vkGetPhysicalDeviceQueueFamilyProperties2(this->physicalDevice, &queuePropertyCount, queueFamilyProperties.data());
    return queueFamilyProperties;
  }
  
  bool VulkanPhysicalDevice::queueFamilySupportsSurfaceKHR(VulkanSurface& surface, uint32_t queueFamily) {
    VkBool32 supportsSurface = false;
    Validation::verify(
      vkGetPhysicalDeviceSurfaceSupportKHR(this->physicalDevice, queueFamily, surface.get(), &supportsSurface)
    );
    return supportsSurface;
  }
}
