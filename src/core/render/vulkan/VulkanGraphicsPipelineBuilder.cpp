#include "radiant/core/render/vulkan/VulkanGraphicsPipelineBuilder.h"
#include <charconv>
#include <slang/slang-com-ptr.h>
#include <slang/slang.h>
#include <vulkan/vulkan_core.h>
#include <array>

namespace Radiant {
  VulkanGraphicsPipelineBuilder::VulkanGraphicsPipelineBuilder() {
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  }

  void VulkanGraphicsPipelineBuilder::withShaderSlang(std::filesystem::path shaderPath) {
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



  }

  void VulkanGraphicsPipelineBuilder::build(VulkanDevice& device) {
    VkShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.codeSize = 0;
    shaderModuleInfo.pCode = nullptr;

    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.pName = "test name";
    shaderStageInfo.module = nullptr;
    shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStageInfo.flags = 0;

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
