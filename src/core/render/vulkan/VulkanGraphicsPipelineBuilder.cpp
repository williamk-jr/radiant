#include "radiant/core/render/vulkan/VulkanGraphicsPipelineBuilder.h"
#include <charconv>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>
#include <vulkan/vulkan_core.h>
#include <array>

namespace Radiant {
  VulkanGraphicsPipelineBuilder::VulkanGraphicsPipelineBuilder(VulkanDevice& device) {
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    this->device = device.get();
  }
  
  void VulkanGraphicsPipelineBuilder::withVertexBindingDescription(uint32_t stride, VkVertexInputRate inputRate, std::vector<VulkanVertexAttributeDescription> attributeDescriptions) {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = this->vertexBindingDescriptors.size();
    bindingDescription.inputRate = inputRate;
    bindingDescription.stride = stride;
    this->vertexBindingDescriptors.push_back(bindingDescription);
    
    // All attributes are tied to the above binding description
    for (int i = 0; i < attributeDescriptions.size(); i++) {
      VkVertexInputAttributeDescription attributeDescription{};
      attributeDescription.binding = this->vertexBindingDescriptors.size();
      attributeDescription.location = this->vertexAttributeDescriptors.size();
      attributeDescription.format = attributeDescriptions[i].format;
      attributeDescription.offset = attributeDescriptions[i].offset;
      this->vertexAttributeDescriptors.push_back(attributeDescription);
    }
  }

  void VulkanGraphicsPipelineBuilder::withInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable) {
    this->inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    this->inputAssemblyStateInfo.primitiveRestartEnable = primitiveRestartEnable;
    this->inputAssemblyStateInfo.topology = topology;
    this->inputAssemblyStateInfo.flags = 0;
  }
  
  void VulkanGraphicsPipelineBuilder::withColorBlendState(std::vector<VkPipelineColorBlendAttachmentState> attachmentStates, float* blendConstants, VkLogicOp logicOperation) {
    this->colorBlendAttachmentStates = attachmentStates;

    this->colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    this->colorBlendStateInfo.attachmentCount = this->colorBlendAttachmentStates.size();
    this->colorBlendStateInfo.pAttachments = this->colorBlendAttachmentStates.data();
    this->colorBlendStateInfo.blendConstants[0] = blendConstants[0];
    this->colorBlendStateInfo.blendConstants[1] = blendConstants[1];
    this->colorBlendStateInfo.blendConstants[2] = blendConstants[2];
    this->colorBlendStateInfo.blendConstants[3] = blendConstants[3];
    this->colorBlendStateInfo.logicOp = logicOperation;
    this->colorBlendStateInfo.flags = 0;
  }
  
  void VulkanGraphicsPipelineBuilder::withDepthStencilState(VulkanDepthTest depthTest, VulkanDepthBoundsTest depthBoundsTest, VulkanStencilTest stencilTest) {
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
  }

  void VulkanGraphicsPipelineBuilder::withShaderSlang(std::string stageName, std::filesystem::path shaderPath, VkShaderStageFlagBits stageFlags) {
    if (this->slangGlobalSession.readRef() == nullptr) {
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
    
    Slang::ComPtr<slang::IModule> slangModule{
      slangSession->loadModuleFromSource("test", shaderPath.c_str(), nullptr)
    };

    Slang::ComPtr<slang::IBlob> spirv;
    slangModule->getTargetCode(0, spirv.writeRef());

    VkShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.codeSize = spirv->getBufferSize();
    shaderModuleInfo.pCode = (uint32_t*)spirv->getBufferPointer();
    
    VkShaderModule shaderModule;
    vkCreateShaderModule(this->device, &shaderModuleInfo, nullptr, &shaderModule);

    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.pName = stageName.c_str();
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.stage = stageFlags;
    shaderStageInfo.flags = 0;

    this->shaderStages.push_back(shaderStageInfo);

  }

  void VulkanGraphicsPipelineBuilder::build(VulkanDevice& device) {
    this->createInfo.stageCount = this->shaderStages.size();
    this->createInfo.pStages = this->shaderStages.data();

    VkPipelineVertexInputStateCreateInfo vertextInputStateInfo{};
    vertextInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertextInputStateInfo.vertexBindingDescriptionCount = this->vertexBindingDescriptors.size();
    vertextInputStateInfo.pVertexBindingDescriptions = this->vertexBindingDescriptors.data();
    vertextInputStateInfo.vertexAttributeDescriptionCount = this->vertexAttributeDescriptors.size();
    vertextInputStateInfo.pVertexAttributeDescriptions = this->vertexAttributeDescriptors.data();
    vertextInputStateInfo.flags = 0;

    this->createInfo.pVertexInputState = &vertextInputStateInfo;
    this->createInfo.pInputAssemblyState = &this->inputAssemblyStateInfo;
    this->createInfo.pColorBlendState = &this->colorBlendStateInfo;
    this->createInfo.pDepthStencilState = &this->depthStencilInfo;

    VkPipeline graphicsPipeline;
    vkCreateGraphicsPipelines(
        device.get(),
        nullptr, 
        1, &this->createInfo, 
        nullptr, 
        &graphicsPipeline
    );
  }
}
