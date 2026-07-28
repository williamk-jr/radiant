#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/pipeline/VulkanPipeline.h"

#include <filesystem>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	struct VulkanVertexAttributeDescription {
			VkFormat format;
			uint32_t offset;
	};

	struct VulkanDepthTest {
			VkBool32    enabled;
			VkBool32    depthWriteEnabled;
			VkCompareOp compare;
	};

	struct VulkanDepthBoundsTest {
			VkBool32 enabled;
			float    minDepthBounds;
			float    maxDepthBounds;
	};

	struct VulkanStencilTest {
			VkBool32         enabled;
			VkStencilOpState front;
			VkStencilOpState back;
	};

	struct VulkanDepthBias {
			VkBool32 enabled;
			VkBool32 clampEnabled;
			float    clamp;
			float    constantfactor;
			float    slopefactor;
	};

	struct VulkanSampleShading {
			VkBool32 enabled;
			float    min;
	};

	class VulkanGraphicsPipelineBuilder {
		public:
			VulkanGraphicsPipelineBuilder(VulkanDevice& device);

			VulkanGraphicsPipelineBuilder& allowDerivatives();
			VulkanGraphicsPipelineBuilder& enableCaching();
			VulkanGraphicsPipelineBuilder& derivativeOf(VulkanPipeline& basePipeline);
			VulkanGraphicsPipelineBuilder& cacheOf(VulkanPipeline& basePipeline);

			VulkanGraphicsPipelineBuilder& withLayout(std::vector<VulkanDescriptorSetLayout>& descriptorSetLayouts);

			VulkanGraphicsPipelineBuilder& withLayout(std::vector<VkDescriptorSetLayout> descriptorSetLayouts);

			VulkanGraphicsPipelineBuilder& withRenderingInfo(std::vector<VkFormat> colorAttachmentFormats,
			                                                 VkFormat              depthAttachmentFormat,
			                                                 VkFormat              stencilAttachmentFormat);

			VulkanGraphicsPipelineBuilder&
			withVertexBindingDescription(uint32_t                                      stride,
			                             VkVertexInputRate                             inputRate,
			                             std::vector<VulkanVertexAttributeDescription> attributeDescriptions);

			VulkanGraphicsPipelineBuilder& withInputAssemblyState(VkPrimitiveTopology topology,
			                                                      VkBool32            primitiveRestartEnable);

			VulkanGraphicsPipelineBuilder& withRasterizationState(VkPolygonMode      polygonMode,
			                                                      VkCullModeFlagBits cullMode,
			                                                      VkFrontFace        frontFace,
			                                                      VulkanDepthBias    depthBias,
			                                                      float              lineWidth,
			                                                      VkBool32           rasterizerDiscardEnable);

			VulkanGraphicsPipelineBuilder& withMultisampleState(VkSampleCountFlagBits rasterizationSampleCount,
			                                                    VulkanSampleShading   sampleShading,
			                                                    VkBool32              alphaToCoverageEnable,
			                                                    VkBool32              alphaToOneEnable);

			VulkanGraphicsPipelineBuilder& withDynamicState(std::vector<VkDynamicState> dynamicStates);

			VulkanGraphicsPipelineBuilder& withViewportState(std::vector<VkViewport> viewports,
			                                                 std::vector<VkRect2D>   scissors);

			VulkanGraphicsPipelineBuilder& withViewportState(uint32_t viewportCount, uint32_t scissorCount);

			VulkanGraphicsPipelineBuilder&
			withColorBlendState(std::vector<VkPipelineColorBlendAttachmentState> attachmentStates,
			                    float                                            blendConstants[4]);

			VulkanGraphicsPipelineBuilder& withColorBlendState(VkLogicOp logicOperation, float blendConstants[4]);

			VulkanGraphicsPipelineBuilder& withDepthStencilState(VulkanDepthTest       depthTest,
			                                                     VulkanDepthBoundsTest depthBoundsTest,
			                                                     VulkanStencilTest     stencilTest);

			VulkanGraphicsPipelineBuilder&
			withShaderSlang(std::string stageName, std::filesystem::path shaderPath, VkShaderStageFlagBits stageFlags);

			// void withShaderSpirv(std::filesystem::path shaderPath);

			VulkanPipeline build();

		private:
			Slang::ComPtr<slang::IGlobalSession> slangGlobalSession;
			Slang::ComPtr<slang::ISession>       slangSession;
			VkGraphicsPipelineCreateInfo         createInfo{};
			VkDevice                             device;

			std::vector<VkDescriptorSetLayout>               descriptorSetLayouts;
			std::vector<VkVertexInputBindingDescription>     vertexBindingDescriptors;
			std::vector<VkVertexInputAttributeDescription>   vertexAttributeDescriptors;
			std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStates;
			std::vector<VkShaderModule>                      shaderModules;
			std::vector<VkPipelineShaderStageCreateInfo>     shaderStages;
			std::vector<VkDynamicState>                      dynamicStates;
			std::vector<VkViewport>                          viewports;
			std::vector<VkRect2D>                            scissors;

			VkPipelineLayout                                        layout;
			VkPipelineCreateFlags2                                  flags = 0;
			std::unique_ptr<VkPipelineCreateFlags2CreateInfo>       createFlags;
			std::unique_ptr<VkPipelineRenderingCreateInfo>          renderingInfo;
			std::unique_ptr<VkPipelineVertexInputStateCreateInfo>   vertextInputStateInfo;
			std::unique_ptr<VkPipelineInputAssemblyStateCreateInfo> inputAssemblyStateInfo;
			std::unique_ptr<VkPipelineRasterizationStateCreateInfo> rasterizationStateInfo;
			std::unique_ptr<VkPipelineColorBlendStateCreateInfo>    colorBlendStateInfo;
			std::unique_ptr<VkPipelineDepthStencilStateCreateInfo>  depthStencilInfo;
			std::unique_ptr<VkPipelineMultisampleStateCreateInfo>   multisampleStateInfo;
			std::unique_ptr<VkPipelineDynamicStateCreateInfo>       dynamicStateInfo;
			std::unique_ptr<VkPipelineViewportStateCreateInfo>      viewportStateInfo;

			VkPipeline      basePipeline = VK_NULL_HANDLE;
			VkPipelineCache cache        = VK_NULL_HANDLE;
	};
} // namespace Radiant
