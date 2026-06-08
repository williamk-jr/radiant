
#pragma once
#include "radiant/core/render/vulkan/VulkanBuffer.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanPipeline.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanCommandPool;

  class VulkanCommandBuffer {
    public:
      VulkanCommandBuffer(VkCommandBuffer commandBuffer);
      VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
      VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;

      VulkanCommandBuffer(VulkanCommandBuffer&&) noexcept;
      VulkanCommandBuffer& operator=(VulkanCommandBuffer&&) noexcept = default;
      VkCommandBuffer get();

      void begin(VkCommandBufferUsageFlags flags);
      void pipelineMemoryBarrier(std::vector<VkMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);
      void pipelineImageMemoryBarrier(std::vector<VkImageMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);
      void pipelineBufferMemoryBarrier(std::vector<VkBufferMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);
      void clearColor(VulkanImage& image, VkClearColorValue& color);
      
      void beginRendering(
          std::vector<VkRenderingAttachmentInfo>* colorAttachments, 
          VkRenderingAttachmentInfo* depthAttachment, 
          VkRenderingAttachmentInfo* stencilAttachment, 
          VkRect2D renderArea, VkRenderingFlags renderingFlags
      );
      void bindPipeline(VulkanPipeline& pipeline);
      void bindVertexBuffer(VulkanBuffer& buffer, uint32_t firstBinding, VkDeviceSize offset);
      void bindIndexBuffer(VulkanBuffer& buffer, VkDeviceSize offset, VkIndexType indexType);
      void drawIndexed(uint32_t indexCount, uint32_t instanceCount);


      void setViewport(float width, float height, float minDepth, float maxDepth);
      void setScissor(uint32_t width, uint32_t height);
      void clearAttachments(std::vector<VkClearAttachment> clearAttachments, std::vector<VkClearRect> clearAreas);
      void clearAttachment(VulkanImage& image, VkClearAttachment clearAttachment);

      void endRendering();


      void end();
      void reset(bool recycleResources);

    private:
      friend class VulkanCommandPool;
      VkCommandBuffer commandBuffer;
      
  };
}
