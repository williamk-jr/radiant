
#pragma once
#include "radiant/core/render/vulkan/VulkanBuffer.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanPipeline.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanCommandPool;

  /* VulkanCommandBuffer
   *
   * A wrapper for VkCOmmandBuffer.
   * Represents a buffer in which commands are recorded to later be submitted to a queue and executed by the physical device.
   */
  class VulkanCommandBuffer {
    public:
      /*
       * @param a valid VkCommandBuffer.
       */
      VulkanCommandBuffer(VkCommandBuffer commandBuffer);
      VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
      VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;

      VulkanCommandBuffer(VulkanCommandBuffer&&) noexcept;
      VulkanCommandBuffer& operator=(VulkanCommandBuffer&&) noexcept = default;

      /*
       * @return A raw VkCommandBuffer.
       */
      VkCommandBuffer get();

      /*
       * Begins recording command buffer.
       *
       * @param VkCommandBufferUsageFlags Usage flags.
       */
      void begin(VkCommandBufferUsageFlags flags);

      /*
       * Inserts memory barriers.
       *
       * @param std::vector<VkMemoryBarrier2>& A reference to a vector of memory barriers.
       * @param VkDependencyFlags Dependency flags. 
       */
      void pipelineMemoryBarrier(std::vector<VkMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);


      /*
       * Inserts image memory barriers.
       *
       * @param std::vector<VkImageMemoryBarrier2>& A reference to a vector of image memory barriers.
       * @param VkDependencyFlags Dependency flags. 
       */
      void pipelineImageMemoryBarrier(std::vector<VkImageMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);

      /*
       * Inserts buffer memory barriers.
       *
       * @param std::vector<VkBufferMemoryBarrier2>& A reference to a vector of buffer memory barriers.
       * @param VkDependencyFlags Dependency flags. 
       */
      void pipelineBufferMemoryBarrier(std::vector<VkBufferMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);

      /*
       * Clears the provided image to a color.
       *
       * @param VulkanImage& A reference to a valid vulkan image.
       * @param VkClearColorValue& A reference to a clear color.
       */
      void clearColor(VulkanImage& image, VkClearColorValue& color);
      
      /*
       * Begins rendering the frame.
       *
       * @param std::vector<VkRenderingAttachmentInfo>* A pointer to a vector of rendering attachments.
       * @param VkRenderingAttachmentInfo* A pointer to a depth attachment.
       * @param VkRenderingAttachmentInfo* A pointer to a stencil attachment.
       * @param VkRect2D The rectangular area in which to render.
       * @param VkRenderingFlags Rendering flags.
       */
      void beginRendering(
          std::vector<VkRenderingAttachmentInfo>* colorAttachments, 
          VkRenderingAttachmentInfo* depthAttachment, 
          VkRenderingAttachmentInfo* stencilAttachment, 
          VkRect2D renderArea, VkRenderingFlags renderingFlags
      );

      /*
       * Binds a pipeline to use for rendering.
       * Must be called while rendering.
       *
       * @param A reference to a valid vulkan pipeline.
       */
      void bindPipeline(VulkanPipeline& pipeline);

      /*
       * Binds a vertex buffer to use for rendering.
       * Must be called while rendering.
       *
       * @param VulkanBuffer& A reference to a valid vulkan buffer.
       * @param uint32_t Index at which to bind the vertex buffer.
       * @param VkDeviceSize Offset in bytes from which to start binding vertex buffer data from.
       */
      void bindVertexBuffer(VulkanBuffer& buffer, uint32_t firstBinding, VkDeviceSize offset);

      /*
       * Binds a vertex buffer to use for rendering.
       * Must be called while rendering.
       *
       * @param VulkanBuffer& A reference to a valid vulkan buffer.
       * @param uint32_t Index at which to bind the vertex buffer.
       * @param VkDeviceSize Offset in bytes from which to start binding vertex buffer data from.
       * @param VkDeviceSize Amount of the buffer in bytes to read starting from the offset.
       */
      void bindVertexBuffer(VulkanBuffer& buffer, uint32_t firstBinding, VkDeviceSize offset, VkDeviceSize size);

      /*
       * Binds an index buffer to use for rendering.
       * Must be called while rendering.
       *
       * @param VulkanBuffer& A reference to a valid vulkan buffer.
       * @param VkDeviceSize Offset in bytes from which to start binding index buffer data from.
       * @param VkIndexType Enum value representing the data type that the index buffer will be made up of.
       */
      void bindIndexBuffer(VulkanBuffer& buffer, VkDeviceSize offset, VkIndexType indexType);

      /*
       * Binds descriptor sets to a pipepline.
       *
       * @param VulkanPipeline& A reference to a valid vulkan pipeline.
       * @param uint32_t Index of the first descriptor set to bind.
       * @param std::vector<VulkanDescriptorSet>& A reference to a vector of descriptor sets.
       */
      void bindDescriptorSets(VulkanPipeline& pipeline, uint32_t firstSet, std::vector<VulkanDescriptorSet>& descriptorSets);

      /*
       * Draws `instanceCount` number of instances with `indexCount` number of indicies.
       *
       * @param uint32_t Number of indicies needed to draw each instance.
       * @param uint32_t Number of instances to draw.
       */
      void drawIndexed(uint32_t indexCount, uint32_t instanceCount);


      /*
       * Sets a viewport.
       *
       * @param float Width of the viewport.
       * @param float Height of the viewport.
       * @param float Minimum depth of the viewport (near).
       * @param float Maximum depth of the viewport (far).
       */
      void setViewport(float width, float height, float minDepth, float maxDepth);

      /*
       * Sets a scissor area.
       *
       * @param uint32_t Width of scissor area.
       * @param uint32_t Height of scissor area.
       */
      void setScissor(uint32_t width, uint32_t height);

      void clearAttachments(std::vector<VkClearAttachment> clearAttachments, std::vector<VkClearRect> clearAreas);

      void clearAttachment(VulkanImage& image, VkClearAttachment clearAttachment);


      /*
       * Stops rendering a frame.
       */
      void endRendering();

      /*
       * Stops recording the command buffer.
       */
      void end();

      /*
       * Resets the command buffer state.
       */
      void reset(bool recycleResources);

    private:
      friend class VulkanCommandPool;
      VkCommandBuffer commandBuffer;
      
  };
}
