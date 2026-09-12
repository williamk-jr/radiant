#include "radiant/core/render/vulkan/VulkanSampler.h"

#include "radiant/core/render/vulkan/VulkanUtil.h"

#include <vulkan/vulkan_core.h>

namespace Radiant {
	VulkanSampler::VulkanSampler(VulkanDevice& device, VkSamplerCreateFlags flags) : device(device.get()) {
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.flags = flags;

		VkResult result = vkCreateSampler(device.get(), &samplerInfo, nullptr, &this->sampler);
		VulkanUtil::validate("VulkanSampler Initialization Error. Failed to create sampler.", result);
	}

	VulkanSampler::VulkanSampler(VulkanSampler&& other) noexcept : sampler(other.sampler), device(other.device) {
		other.sampler = nullptr;
	}

	VulkanSampler::~VulkanSampler() {
		vkDestroySampler(this->device, this->sampler, nullptr);
	}

	VkSampler VulkanSampler::get() {
		return this->sampler;
	}
} // namespace Radiant
