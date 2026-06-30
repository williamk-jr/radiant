#include "radiant/core/render/vulkan/VulkanGraphicsPipelineBuilder.h"
#include "radiant/util/logger/Logger.h"
#include <filesystem>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>
#include <string>
#include <vulkan/vulkan_core.h>
#include <array>

namespace Radiant {
  VulkanGraphicsPipelineBuilder::VulkanGraphicsPipelineBuilder(VulkanDevice& device) {
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    this->device = device.get();
  }
  
  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withLayout(std::vector<VulkanDescriptorSetLayout>& descriptorSetLayouts) {
    for (VulkanDescriptorSetLayout& descriptorSetLayout : descriptorSetLayouts) {
      this->descriptorSetLayouts.push_back(descriptorSetLayout.get());
    }
    
    VkPipelineLayoutCreateInfo pipelineLayoutinfo{};
    pipelineLayoutinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutinfo.setLayoutCount = this->descriptorSetLayouts.size();
    pipelineLayoutinfo.pSetLayouts = this->descriptorSetLayouts.data();
    pipelineLayoutinfo.flags = 0;

    vkCreatePipelineLayout(this->device, &pipelineLayoutinfo, nullptr, &this->layout);
    return *this;
  }

  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withRenderingInfo(std::vector<VkFormat> colorAttachmentFormats, VkFormat depthAttachmentFormat, VkFormat stencilAttachmentFormat) {
    this->renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    this->renderingInfo.colorAttachmentCount = colorAttachmentFormats.size();
    this->renderingInfo.pColorAttachmentFormats = colorAttachmentFormats.data();
    this->renderingInfo.depthAttachmentFormat = depthAttachmentFormat;
    this->renderingInfo.stencilAttachmentFormat = stencilAttachmentFormat;
    return *this;
  }

  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withVertexBindingDescription(uint32_t stride, VkVertexInputRate inputRate, std::vector<VulkanVertexAttributeDescription> attributeDescriptions) {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = this->vertexBindingDescriptors.size();
    bindingDescription.inputRate = inputRate;
    bindingDescription.stride = stride;
    
    // All attributes are tied to the above binding description
    for (int i = 0; i < attributeDescriptions.size(); i++) {
      VkVertexInputAttributeDescription attributeDescription{};
      attributeDescription.binding = this->vertexBindingDescriptors.size();
      attributeDescription.location = this->vertexAttributeDescriptors.size();
      attributeDescription.format = attributeDescriptions[i].format;
      attributeDescription.offset = attributeDescriptions[i].offset;
      this->vertexAttributeDescriptors.push_back(attributeDescription);
    }
    this->vertexBindingDescriptors.push_back(bindingDescription); // Don't move before loop. 
    return *this;
  }

  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable) {
    this->inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    this->inputAssemblyStateInfo.primitiveRestartEnable = primitiveRestartEnable;
    this->inputAssemblyStateInfo.topology = topology;
    this->inputAssemblyStateInfo.flags = 0;
    return *this;
  }

  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withRasterizationState(VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkFrontFace frontFace, VulkanDepthBias depthBias, float lineWidth, VkBool32 rasterizerDiscardEnable) {
    this->rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    this->rasterizationStateInfo.cullMode = cullMode;
    this->rasterizationStateInfo.polygonMode = polygonMode;
    this->rasterizationStateInfo.lineWidth = lineWidth;
    this->rasterizationStateInfo.frontFace = frontFace;
    this->rasterizationStateInfo.depthBiasEnable = depthBias.enabled;
    this->rasterizationStateInfo.depthClampEnable = depthBias.clampEnabled;
    this->rasterizationStateInfo.depthBiasClamp = depthBias.clamp;
    this->rasterizationStateInfo.depthBiasConstantFactor = depthBias.constantfactor;
    this->rasterizationStateInfo.depthBiasSlopeFactor = depthBias.slopefactor;
    this->rasterizationStateInfo.rasterizerDiscardEnable = rasterizerDiscardEnable;
    this->rasterizationStateInfo.flags = 0;
    return *this;
  }
  
  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withMultisampleState(VkSampleCountFlagBits rasterizationSampleCount, VulkanSampleShading sampleShading, VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable) {
    this->multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    this->multisampleStateInfo.rasterizationSamples = rasterizationSampleCount;
    this->multisampleStateInfo.pSampleMask = nullptr; // TODO implement sample masks
    this->multisampleStateInfo.sampleShadingEnable = sampleShading.enabled;
    this->multisampleStateInfo.minSampleShading = sampleShading.min;
    this->multisampleStateInfo.alphaToCoverageEnable = alphaToCoverageEnable;
    this->multisampleStateInfo.alphaToOneEnable = alphaToOneEnable;
    this->multisampleStateInfo.flags = 0;
    return *this;
  }

  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withDynamicState(std::vector<VkDynamicState> dynamicStates) {
    this->dynamicStates = dynamicStates;

    this->dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    this->dynamicStateInfo.dynamicStateCount = this->dynamicStates.size();
    this->dynamicStateInfo.pDynamicStates = this->dynamicStates.data();
    this->dynamicStateInfo.flags = 0;
    return *this;
  }
  
  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withViewportState(std::vector<VkViewport> viewports, std::vector<VkRect2D> scissors) {
    this->viewports = viewports;
    this->scissors = scissors;

    this->viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    this->viewportStateInfo.viewportCount = this->viewports.size();
    this->viewportStateInfo.pViewports = this->viewports.data();
    this->viewportStateInfo.scissorCount = this->scissors.size();
    this->viewportStateInfo.pScissors = this->scissors.data();
    this->viewportStateInfo.flags = 0;
    return *this;
  }

  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withViewportState(uint32_t viewportCount, uint32_t scissorCount) {
    this->viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    this->viewportStateInfo.viewportCount = viewportCount;
    this->viewportStateInfo.scissorCount = scissorCount;
    this->viewportStateInfo.flags = 0;
    return *this;
  }
  
  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withColorBlendState(std::vector<VkPipelineColorBlendAttachmentState> attachmentStates, float* blendConstants, VkLogicOp logicOperation) {
    this->colorBlendAttachmentStates = attachmentStates;

    this->colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    this->colorBlendStateInfo.attachmentCount = this->colorBlendAttachmentStates.size();
    this->colorBlendStateInfo.pAttachments = this->colorBlendAttachmentStates.data();
    if (blendConstants != nullptr) {
      this->colorBlendStateInfo.blendConstants[0] = blendConstants[0];
      this->colorBlendStateInfo.blendConstants[1] = blendConstants[1];
      this->colorBlendStateInfo.blendConstants[2] = blendConstants[2];
      this->colorBlendStateInfo.blendConstants[3] = blendConstants[3];
    } else {
      this->colorBlendStateInfo.blendConstants[0] = 0;
      this->colorBlendStateInfo.blendConstants[1] = 0;
      this->colorBlendStateInfo.blendConstants[2] = 0;
      this->colorBlendStateInfo.blendConstants[3] = 0;
    }
    this->colorBlendStateInfo.logicOpEnable = VK_FALSE;
    this->colorBlendStateInfo.logicOp = logicOperation;
    this->colorBlendStateInfo.flags = 0;
    return *this;
  }
  
  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withDepthStencilState(VulkanDepthTest depthTest, VulkanDepthBoundsTest depthBoundsTest, VulkanStencilTest stencilTest) {
    this->depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    this->depthStencilInfo.depthTestEnable = depthTest.enabled;
    this->depthStencilInfo.depthWriteEnable = depthTest.depthWriteEnabled;
    this->depthStencilInfo.depthCompareOp = depthTest.compare;
    this->depthStencilInfo.depthBoundsTestEnable = depthBoundsTest.enabled;
    this->depthStencilInfo.minDepthBounds = depthBoundsTest.minDepthBounds;
    this->depthStencilInfo.maxDepthBounds = depthBoundsTest.maxDepthBounds;
    this->depthStencilInfo.front = stencilTest.front;
    this->depthStencilInfo.back = stencilTest.back;
    this->depthStencilInfo.flags = 0;
    return *this;
  }

  VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::withShaderSlang(std::string stageName, std::filesystem::path shaderPath, VkShaderStageFlagBits stageFlags) {
    if (!std::filesystem::exists(shaderPath)) {
      Logger::error("Could not find shader file: " + std::filesystem::absolute(shaderPath).string());
    }

    if (this->slangGlobalSession.get() == nullptr) {
      //Logger::info("Creating slang session.");
      slang::createGlobalSession(slangGlobalSession.writeRef());

      slang::TargetDesc slangTargetDesc{};
      slangTargetDesc.format = SLANG_SPIRV;
      slangTargetDesc.profile = slangGlobalSession->findProfile("spirv_1_4");
      std::array<slang::TargetDesc, 1> slangTargets{slangTargetDesc};

      slang::CompilerOptionEntry slangCompilerOptionEntry{};
      slangCompilerOptionEntry.name = slang::CompilerOptionName::EmitSpirvDirectly;
      slangCompilerOptionEntry.value = {slang::CompilerOptionValueKind::Int, 1};
      std::array<slang::CompilerOptionEntry, 1> slangCompilerOptions{slangCompilerOptionEntry};

      slang::SessionDesc slangSessionDesc{};
      slangSessionDesc.targetCount = slangTargets.size();
      slangSessionDesc.targets = slangTargets.data();
      slangSessionDesc.compilerOptionEntryCount = slangCompilerOptions.size();
      slangSessionDesc.compilerOptionEntries = slangCompilerOptions.data();
      slangSessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR; // Matches matricies in glm library
      
      slangGlobalSession->createSession(slangSessionDesc, this->slangSession.writeRef());
    }
    
    const char* cShaderPath = shaderPath.c_str();
    Slang::ComPtr<slang::IBlob> diagnostics;
    Slang::ComPtr<slang::IModule> slangModule{
      slangSession->loadModuleFromSource("test", cShaderPath, nullptr, diagnostics.writeRef())
    };
    
    if (diagnostics != nullptr) {
      const char* message = (const char*) diagnostics->getBufferPointer();
      Logger::error(message);
    }

    Slang::ComPtr<slang::IBlob> spirv;
    slangModule->getTargetCode(0, spirv.writeRef());

    VkShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.codeSize = spirv->getBufferSize();
    shaderModuleInfo.pCode = (uint32_t*)spirv->getBufferPointer();
    
    VkShaderModule shaderModule;
    vkCreateShaderModule(this->device, &shaderModuleInfo, nullptr, &shaderModule);
    this->shaderModules.push_back(shaderModule);

    const char* cStageName = stageName.c_str();
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.pName = cStageName;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.stage = stageFlags;
    shaderStageInfo.flags = 0;

    this->shaderStages.push_back(shaderStageInfo);
    return *this;
  }

  VulkanPipeline VulkanGraphicsPipelineBuilder::build() {
    this->createInfo.stageCount = this->shaderStages.size();
    this->createInfo.pStages = this->shaderStages.data();

    this->vertextInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    this->vertextInputStateInfo.vertexBindingDescriptionCount = this->vertexBindingDescriptors.size();
    this->vertextInputStateInfo.pVertexBindingDescriptions = this->vertexBindingDescriptors.data();
    this->vertextInputStateInfo.vertexAttributeDescriptionCount = this->vertexAttributeDescriptors.size();
    this->vertextInputStateInfo.pVertexAttributeDescriptions = this->vertexAttributeDescriptors.data();
    this->vertextInputStateInfo.flags = 0;

    this->createInfo.layout = this->layout;
    this->createInfo.pVertexInputState = &this->vertextInputStateInfo;
    this->createInfo.pInputAssemblyState = &this->inputAssemblyStateInfo;
    this->createInfo.pRasterizationState = &this->rasterizationStateInfo;
    this->createInfo.pColorBlendState = &this->colorBlendStateInfo;
    this->createInfo.pDepthStencilState = &this->depthStencilInfo;
    this->createInfo.pMultisampleState = &this->multisampleStateInfo;
    this->createInfo.pDynamicState = &this->dynamicStateInfo;
    this->createInfo.pViewportState = &this->viewportStateInfo;
    this->createInfo.pNext = &this->renderingInfo; 


    VkPipeline graphicsPipeline;
    vkCreateGraphicsPipelines(
        this->device,
        nullptr, 
        1, &this->createInfo, 
        nullptr, 
        &graphicsPipeline
    );

    return {this->device, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline, this->layout, this->shaderModules};
  }
}
