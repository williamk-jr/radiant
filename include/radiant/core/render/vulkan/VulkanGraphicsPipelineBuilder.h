#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <filesystem>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>

namespace Radiant {
  struct VulkanVertexAttributeDescription {
    VkFormat format;
    uint32_t offset;
  };

  struct VulkanDepthTest {
    VkBool32 enabled;
    VkBool32 depthWriteEnabled;
    VkCompareOp compare;
  };

  struct VulkanDepthBoundsTest {
    VkBool32 enabled;
    float minDepthBounds;
    float maxDepthBounds;
  };

  struct VulkanStencilTest {
    VkBool32 enabled;
    VkStencilOpState front;
    VkStencilOpState back;
  };

  class VulkanGraphicsPipelineBuilder {
    public:
      VulkanGraphicsPipelineBuilder(VulkanDevice& device);

      void withVertexBindingDescription(uint32_t stride, VkVertexInputRate inputRate, std::vector<VulkanVertexAttributeDescription> attributeDescriptions);
      void withInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);
      void withColorBlendState(std::vector<VkPipelineColorBlendAttachmentState> attachmentStates, float blendConstants[4], VkLogicOp logicOperation);
      void withDepthStencilState(VulkanDepthTest depthTest, VulkanDepthBoundsTest depthBoundsTest, VulkanStencilTest stencilTest); 
    
      void withShaderSlang(std::string stageName, std::filesystem::path shaderPath, VkShaderStageFlagBits stageFlags);
      void withShaderSpirv(std::filesystem::path shaderPath);

      void build(VulkanDevice& device);
    private:
      Slang::ComPtr<slang::IGlobalSession> slangGlobalSession;
      Slang::ComPtr<slang::ISession> slangSession;
      VkGraphicsPipelineCreateInfo createInfo{};
      VkDevice device;

      std::vector<VkVertexInputBindingDescription> vertexBindingDescriptors;
      std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptors;
      std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStates;
      std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

      VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo{};
      VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};
      VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
  };
}

