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

    std::runtime_error("Unable to find a device that meets the provided requirements.");
  }

  VulkanPhysicalDevice::~VulkanPhysicalDevice() {
    
  }

  VkPhysicalDevice& VulkanPhysicalDevice::get() {
    return this->physicalDevice;
  }

  // TODO figure out scoping of this method
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

  uint32_t VulkanPhysicalDevice::getQueueFamily(
    std::vector<VkQueueFamilyProperties2> queueFamilies, 
    VulkanQueueFamilyRequirements queueFamilyRequirements
  ) { 

    for (int i = 0; i < queueFamilies.size(); i++) {
      if (queueFamilyRequirements(queueFamilies[i])) {
        return i;
      }
    }
    return -1;

  }
}
