#include "radiant/core/render/vulkan/VulkanCommandPool.h"

#include "radiant/core/render/vulkan/VulkanResult.h"

#include <span>
#include <vector>

namespace Radiant {
	VulkanCommandPool::VulkanCommandPool(VulkanDevice& device, uint32_t queueFamily) : device(device.get()) {
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = queueFamily;
		commandPoolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // TODO research flags more

		Validation::verify(vkCreateCommandPool(device.get(), &commandPoolInfo, nullptr, &this->commandPool));
	}

	VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& other) noexcept
	    : commandPool(other.commandPool), device(other.device) {
		other.commandPool = nullptr;
	}

	VulkanCommandPool::~VulkanCommandPool() {
		vkDestroyCommandPool(this->device, this->commandPool, nullptr);
	}

	VulkanResult<void> VulkanCommandPool::reset(bool recycleResources) {
		return vkResetCommandPool(this->device, this->commandPool,
		                          recycleResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0);
	}

	VulkanResult<std::vector<VulkanCommandBuffer>>
	VulkanCommandPool::allocateCommandBuffers(int count, VkCommandBufferLevel level) {
		std::vector<VkCommandBuffer> commandBuffers(count);

		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool        = this->commandPool;
		commandBufferAllocateInfo.commandBufferCount = count;
		commandBufferAllocateInfo.level              = level;

		VkResult result = vkAllocateCommandBuffers(this->device, &commandBufferAllocateInfo, commandBuffers.data());

		std::vector<VulkanCommandBuffer> wrappedCommandBuffers;
		wrappedCommandBuffers.reserve(count);

		for (auto buffer : commandBuffers) {
			wrappedCommandBuffers.emplace_back(this->device, buffer, this->commandPool);
		}

		return {result, std::move(wrappedCommandBuffers)};
	}

	VulkanResult<VulkanCommandBuffer> VulkanCommandPool::allocateCommandBuffer(VkCommandBufferLevel level) {
		VulkanResult<std::vector<VulkanCommandBuffer>> result = this->allocateCommandBuffers(1, level);
		return {result.getResult(), std::move(result.getValue()[0])};
	}

	void VulkanCommandPool::freeCommandBuffers(std::span<VulkanCommandBuffer> commandBuffers) {
		VkCommandBuffer* rawCommandBuffers = new VkCommandBuffer[commandBuffers.size()];

		for (int i = 0; i < commandBuffers.size(); i++) {
			rawCommandBuffers[i] = commandBuffers[i].get();
		}

		vkFreeCommandBuffers(this->device, this->commandPool, commandBuffers.size(), rawCommandBuffers);
	}
} // namespace Radiant
