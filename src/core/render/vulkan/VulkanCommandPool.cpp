
#include "radiant/core/render/vulkan/VulkanCommandPool.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>
namespace Radiant {
  VulkanCommandPool::VulkanCommandPool(VulkanDevice& device, uint32_t queueFamily) : device(device) {
    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.queueFamilyIndex = queueFamily;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    Validation::verify(
      vkCreateCommandPool(device.get(), &commandPoolInfo, nullptr, &this->commandPool)
    ); 
  }
  
  VulkanCommandPool::~VulkanCommandPool() {
    vkDestroyCommandPool(device.get(), this->commandPool, nullptr);
  }
}
