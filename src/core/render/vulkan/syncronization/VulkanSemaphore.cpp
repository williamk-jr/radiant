#include "radiant/core/render/vulkan/syncronization/VulkanSemaphore.h"

#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
	VulkanSemaphore::VulkanSemaphore(VulkanDevice& device, VkSemaphoreTypeCreateInfo type, VkSemaphoreCreateFlags flags)
	    : device(device.get()) {
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.flags = flags;
		semaphoreInfo.pNext = &type;

		VkResult result = vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &this->semaphore);
		VulkanUtil::validate("VulkanSemaphore Initialization Error. Failed to create semaphore.", result);
	}

	VulkanSemaphore::VulkanSemaphore(VulkanSemaphore&& other) noexcept
	    : semaphore(other.semaphore), device(other.device) {
		other.semaphore = nullptr;
		other.device    = nullptr;
	}

	VulkanSemaphore::~VulkanSemaphore() {
		vkDestroySemaphore(this->device, this->semaphore, nullptr);
	}

	VkSemaphore VulkanSemaphore::get() const {
		return this->semaphore;
	}
} // namespace Radiant
