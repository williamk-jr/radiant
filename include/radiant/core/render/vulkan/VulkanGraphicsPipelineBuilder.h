#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <filesystem>
#include <vulkan/vulkan_core.h>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>

namespace Radiant {
  class VulkanGraphicsPipelineBuilder {
    public:
      VulkanGraphicsPipelineBuilder();
    
      void withShaderSlang(std::filesystem::path shaderPath);
      void withShaderSpirv(std::filesystem::path shaderPath);

      void build(VulkanDevice& device);
    private:
      Slang::ComPtr<slang::IGlobalSession> slangGlobalSession;
      Slang::ComPtr<slang::ISession> slangSession;
      VkGraphicsPipelineCreateInfo createInfo{};
  };
}

