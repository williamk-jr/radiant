#include "radiant/core/render/vulkan/VulkanQueue.h"

#include <vulkan/vulkan_core.h>

namespace Radiant {
	VulkanQueue::VulkanQueue(VulkanDevice& device, uint32_t queueFamily, uint32_t queueIndex) {
		vkGetDeviceQueue(device.get(), queueFamily, queueIndex, &this->queue);
	}

	VulkanQueue::VulkanQueue(VulkanQueue&& other) noexcept : queue(other.queue) {
		other.queue = nullptr;
	}

	VulkanResult<void> VulkanQueue::submit(std::span<VulkanCommandBuffer>         commandBuffers,
	                                       std::vector<VulkanSemaphoreSubmitInfo> waitSemaphores,
	                                       std::vector<VulkanSemaphoreSubmitInfo> signalSemaphores,
	                                       VulkanFence*                           fence) {
		std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfos;
		commandBufferSubmitInfos.reserve(commandBuffers.size());

		for (VulkanCommandBuffer& commandBuffer : commandBuffers) {
			VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
			commandBufferSubmitInfo.sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
			commandBufferSubmitInfo.commandBuffer = commandBuffer.get();

			commandBufferSubmitInfos.push_back(commandBufferSubmitInfo);
		}

		VkSubmitInfo2 submitInfo{};
		submitInfo.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		submitInfo.commandBufferInfoCount = commandBufferSubmitInfos.size();
		submitInfo.pCommandBufferInfos    = commandBufferSubmitInfos.data();

		// Wait semaphores
		std::vector<VkSemaphoreSubmitInfo> waitSemaphoreInfos;
		if (!waitSemaphores.empty()) {
			waitSemaphoreInfos.reserve(waitSemaphores.size());

			for (VulkanSemaphoreSubmitInfo& i : waitSemaphores) {
				waitSemaphoreInfos.push_back(i);
			}

			submitInfo.waitSemaphoreInfoCount = waitSemaphoreInfos.size();
			submitInfo.pWaitSemaphoreInfos    = waitSemaphoreInfos.data();
		}

		// Signal semaphores
		std::vector<VkSemaphoreSubmitInfo> signalSemaphoreInfos;
		if (!waitSemaphores.empty()) {
			signalSemaphoreInfos.reserve(signalSemaphores.size());

			for (VulkanSemaphoreSubmitInfo& i : signalSemaphores) {
				signalSemaphoreInfos.push_back(i);
			}

			submitInfo.signalSemaphoreInfoCount = signalSemaphoreInfos.size();
			submitInfo.pSignalSemaphoreInfos    = signalSemaphoreInfos.data();
		}

		return vkQueueSubmit2(this->queue, 1, &submitInfo, fence == nullptr ? nullptr : fence->get());
	}

	VulkanResult<void> VulkanQueue::submit(VulkanCommandBuffer&                   commandBuffer,
	                                       std::vector<VulkanSemaphoreSubmitInfo> waitSemaphores,
	                                       std::vector<VulkanSemaphoreSubmitInfo> signalSemaphores,
	                                       VulkanFence*                           fence) {
		return this->submit(std::span{&commandBuffer, 1}, waitSemaphores, signalSemaphores, fence);
	}

	VulkanResult<void> VulkanQueue::present(VulkanSwapchain&           swapchain,
	                                        std::vector<uint32_t>      imageIndicies,
	                                        std::span<VulkanSemaphore> waitSemaphores) {
		std::vector<VkSemaphore> rawSemaphore;
		rawSemaphore.reserve(waitSemaphores.size());

		for (VulkanSemaphore& semaphore : waitSemaphores) {
			rawSemaphore.emplace_back(semaphore.get());
		}

		std::vector<VkSwapchainKHR> swapchains{swapchain.get()}; // TODO Expand to allow more than once swapchain.

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores    = rawSemaphore.data();
		presentInfo.pImageIndices      = imageIndicies.data();
		presentInfo.swapchainCount     = swapchains.size();
		presentInfo.pSwapchains        = swapchains.data();

		return vkQueuePresentKHR(this->queue, &presentInfo);
	}

	VulkanResult<void> VulkanQueue::present(VulkanSwapchain&      swapchain,
	                                        std::vector<uint32_t> imageIndicies,
	                                        VulkanSemaphore&      waitSemaphore) {
		return this->present(swapchain, imageIndicies, std::span{&waitSemaphore, 1});
	}

	void VulkanQueue::waitIdle() {
		vkQueueWaitIdle(this->queue);
	}
} // namespace Radiant
