#include "radiant/core/render/Texture.h"
#include "radiant/core/render/vulkan/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <memory>
#include <vector>

namespace Radiant {
  Texture::Texture(VulkanDevice& device, VulkanMemoryAllocator& memoryAllocator, VulkanDescriptorPool& descriptorPool, VulkanCommandPool& commandPool, VulkanQueue& queue, void* buffer, uint32_t width, uint32_t height, uint32_t pixelSize) {
    uint32_t size = width*height*pixelSize;

    // CPU staging buffer.
    VulkanBuffer stagingBuffer(memoryAllocator, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, {});
    stagingBuffer.append(buffer, size);

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    // GPU image.
    this->image = std::make_unique<VulkanImage>(
        memoryAllocator, VkExtent3D{width, height, 1}, 
        VK_FORMAT_A8_UNORM, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    );

    this->imageView = std::make_unique<VulkanImageView>(device, *image, VK_FORMAT_A8_UNORM, subresourceRange);
    this->sampler = std::make_unique<VulkanSampler>(device, 0);

    VkImageSubresourceLayers subresource{};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.baseArrayLayer = 0;
    subresource.layerCount = 1;
    subresource.mipLevel = 0;

    // Region to copy from buffer to image.
    VkBufferImageCopy copyRegion{};
    copyRegion.bufferRowLength = width;
    copyRegion.bufferImageHeight = height;
    copyRegion.bufferOffset = 0;

    copyRegion.imageExtent = {width, height, 1};
    copyRegion.imageOffset = {0,0,0};
    copyRegion.imageSubresource = subresource;

    // Preform the copy.
    VulkanCommandBuffer commandBuffer = commandPool.allocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    commandBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    // Transfer image layout to transfer dst optimal.
    VkImageMemoryBarrier2 toDstOptimal{};
    toDstOptimal.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    toDstOptimal.image = image->get();
    toDstOptimal.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    toDstOptimal.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    toDstOptimal.srcAccessMask = 0;
    toDstOptimal.dstAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT | VK_ACCESS_2_TRANSFER_WRITE_BIT;
    toDstOptimal.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
    toDstOptimal.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    toDstOptimal.subresourceRange = subresourceRange;

    std::vector<VkImageMemoryBarrier2> transferImageMemoryBarriers{toDstOptimal};
    commandBuffer.pipelineImageMemoryBarrier(transferImageMemoryBarriers, 0);

    std::vector<VkBufferImageCopy> copyRegions{copyRegion};
    commandBuffer.copyBufferToImage(
        stagingBuffer, *image, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
        copyRegions
    );

    // Transfer image layout to shader read-only optimal.
    VkImageMemoryBarrier2 toShaderReadOnlyOptimal{};
    toShaderReadOnlyOptimal.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    toShaderReadOnlyOptimal.image = image->get();
    toShaderReadOnlyOptimal.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    toShaderReadOnlyOptimal.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    toShaderReadOnlyOptimal.srcAccessMask = VK_ACCESS_2_TRANSFER_READ_BIT | VK_ACCESS_2_TRANSFER_WRITE_BIT;
    toShaderReadOnlyOptimal.dstAccessMask = VK_ACCESS_2_SHADER_READ_BIT;
    toShaderReadOnlyOptimal.srcStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    toShaderReadOnlyOptimal.dstStageMask = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;
    toShaderReadOnlyOptimal.subresourceRange = subresourceRange;

    std::vector<VkImageMemoryBarrier2> shaderImageMemoryBarriers{toDstOptimal};
    commandBuffer.pipelineImageMemoryBarrier(shaderImageMemoryBarriers, 0);

    // End and submit.
    commandBuffer.end();
    queue.submit(commandBuffer, {}, {}, nullptr);
    queue.waitIdle();

    // Init descriptor set
    this->descriptorSetLayout = std::make_unique<VulkanDescriptorSetLayout>(device, std::vector<VkDescriptorSetLayoutBinding>{
        VkDescriptorSetLayoutBinding{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr}
    }, 0);

    this->descriptorSet = std::make_unique<VulkanDescriptorSet>(
      descriptorPool.allocateDescriptorSet(*this->descriptorSetLayout)
    );

    descriptorPool.updateDescriptorSets({
      VulkanWriteDescriptorSet{this->descriptorSet->get(), 0, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {}, {
        VkDescriptorImageInfo{this->sampler->get(), this->imageView->get(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL}
      }, {}}
    });
  }

  Texture::Texture(Texture&& other) noexcept :
    image(std::move(other.image)), 
    imageView(std::move(other.imageView)), 
    sampler(std::move(other.sampler)), 
    descriptorSetLayout(std::move(other.descriptorSetLayout)),
    descriptorSet(std::move(other.descriptorSet)) {

    this->image = nullptr;
    this->imageView = nullptr;
    this->sampler = nullptr;
    this->descriptorSetLayout = nullptr;
    this->descriptorSet = nullptr;
  }
}
