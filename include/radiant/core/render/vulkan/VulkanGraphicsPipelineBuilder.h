#pragma once

#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanPipeline.h"
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

  struct VulkanDepthBias {
    VkBool32 enabled;
    VkBool32 clampEnabled;
    float clamp;
    float constantfactor;
    float slopefactor;
  };

  struct VulkanSampleShading {
    VkBool32 enabled;
    float min;
  };

  class VulkanGraphicsPipelineBuilder {
    public:
      VulkanGraphicsPipelineBuilder(VulkanDevice& device);

      VulkanGraphicsPipelineBuilder& withLayout(
          std::vector<VulkanDescriptorSetLayout> descriptorSetLayouts
      );

      VulkanGraphicsPipelineBuilder& withRenderingInfo(
          std::vector<VkFormat> colorAttachmentFormats, 
          VkFormat depthAttachmentFormat, 
          VkFormat stencilAttachmentFormat
      );

      VulkanGraphicsPipelineBuilder& withVertexBindingDescription(
          uint32_t stride, 
          VkVertexInputRate inputRate, 
          std::vector<VulkanVertexAttributeDescription> attributeDescriptions
      );

      VulkanGraphicsPipelineBuilder& withInputAssemblyState(
          VkPrimitiveTopology topology, 
          VkBool32 primitiveRestartEnable
      );

      VulkanGraphicsPipelineBuilder& withRasterizationState(
          VkPolygonMode polygonMode, 
          VkCullModeFlagBits cullMode, 
          VkFrontFace frontFace, 
          VulkanDepthBias depthBias, 
          float lineWidth, 
          VkBool32 rasterizerDiscardEnable
      );

      VulkanGraphicsPipelineBuilder& withMultisampleState(
          VkSampleCountFlagBits rasterizationSampleCount, 
          VulkanSampleShading sampleShading, 
          VkBool32 alphaToCoverageEnable, 
          VkBool32 alphaToOneEnable
      );

      VulkanGraphicsPipelineBuilder& withDynamicState(
          std::vector<VkDynamicState> dynamicStates
      );

      VulkanGraphicsPipelineBuilder& withViewportState(
          std::vector<VkViewport> viewports, 
          std::vector<VkRect2D> scissors
      );

      VulkanGraphicsPipelineBuilder& withViewportState(
          uint32_t viewportCount, 
          uint32_t scissorCount
      );

      VulkanGraphicsPipelineBuilder& withColorBlendState(
          std::vector<VkPipelineColorBlendAttachmentState> attachmentStates, 
          float blendConstants[4], 
          VkLogicOp logicOperation
      );

      VulkanGraphicsPipelineBuilder& withDepthStencilState(
          VulkanDepthTest depthTest, 
          VulkanDepthBoundsTest depthBoundsTest, 
          VulkanStencilTest stencilTest
      ); 
    
      VulkanGraphicsPipelineBuilder& withShaderSlang(
          std::string stageName, 
          std::filesystem::path shaderPath, 
          VkShaderStageFlagBits stageFlags
      );

      //void withShaderSpirv(std::filesystem::path shaderPath);

      VulkanPipeline build();
    private:
      Slang::ComPtr<slang::IGlobalSession> slangGlobalSession;
      Slang::ComPtr<slang::ISession> slangSession;
      VkGraphicsPipelineCreateInfo createInfo{};
      VkDevice device;

      std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
      std::vector<VkVertexInputBindingDescription> vertexBindingDescriptors;
      std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptors;
      std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStates;
      std::vector<VkShaderModule> shaderModules;
      std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
      std::vector<VkDynamicState> dynamicStates;
      std::vector<VkViewport> viewports;
      std::vector<VkRect2D> scissors;

      VkPipelineLayout layout;
      VkPipelineRenderingCreateInfo renderingInfo{};
      VkPipelineVertexInputStateCreateInfo vertextInputStateInfo{};
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo{};
      VkPipelineRasterizationStateCreateInfo rasterizationStateInfo{};
      VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};
      VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
      VkPipelineMultisampleStateCreateInfo multisampleStateInfo{};
      VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
      VkPipelineViewportStateCreateInfo viewportStateInfo{};

  };
}

