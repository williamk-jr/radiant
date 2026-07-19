#include "radiant/core/render/vulkan/VulkanSampler.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanSampler::VulkanSampler(VulkanDevice& device, VkSamplerCreateFlags flags) {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.flags = flags;

    vkCreateSampler(device.get(), &samplerInfo, nullptr, &this->sampler);
  }
  
  VulkanSampler::VulkanSampler(VulkanSampler&& other) noexcept :
    sampler(other.sampler), device(other.device) {
    other.sampler = nullptr;
  }

  VulkanSampler::~VulkanSampler() {
    vkDestroySampler(this->device, this->sampler, nullptr);
  }

  VkSampler VulkanSampler::get() {
    return this->sampler;
  }   
}
