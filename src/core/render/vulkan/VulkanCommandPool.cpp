
#include "radiant/core/render/vulkan/VulkanCommandPool.h"

namespace Radiant {
  VulkanCommandPool::VulkanCommandPool(VulkanDevice& device, uint32_t queueFamily) : device(device.get()) {
    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.queueFamilyIndex = queueFamily;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // TODO research flags more

    Validation::verify(
      vkCreateCommandPool(device.get(), &commandPoolInfo, nullptr, &this->commandPool)
    ); 
  }

  VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& other) noexcept :
    commandPool(other.commandPool), device(other.device) {
    other.commandPool = nullptr;
  }
  
  VulkanCommandPool::~VulkanCommandPool() {
    vkDestroyCommandPool(this->device, this->commandPool, nullptr);
  }

  void VulkanCommandPool::reset(bool recycleResources) {
    Validation::verify(
      vkResetCommandPool(this->device, this->commandPool, recycleResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0)
    );
  }
  
  std::vector<VulkanCommandBuffer> VulkanCommandPool::allocateCommandBuffers(int count, VkCommandBufferLevel level) {
    std::vector<VkCommandBuffer> commandBuffers(count);

    VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = this->commandPool;
    commandBufferAllocateInfo.commandBufferCount = count;
    commandBufferAllocateInfo.level = level;

    Validation::verify(
      vkAllocateCommandBuffers(this->device, &commandBufferAllocateInfo, commandBuffers.data())
    );

    std::vector<VulkanCommandBuffer> wrappedCommandBuffers;
    wrappedCommandBuffers.reserve(count);

    for (auto buffer : commandBuffers) {
      wrappedCommandBuffers.emplace_back(buffer);
    }

    return wrappedCommandBuffers;
  }

  void VulkanCommandPool::freeCommandBuffers(std::vector<VulkanCommandBuffer> commandBuffers) {
    VkCommandBuffer* rawCommandBuffers = new VkCommandBuffer[commandBuffers.size()];

    for (int i = 0; i < commandBuffers.size(); i++) {
      rawCommandBuffers[i] = commandBuffers[i].get();
    }

    vkFreeCommandBuffers(this->device, this->commandPool, commandBuffers.size(), rawCommandBuffers);
  }
}
