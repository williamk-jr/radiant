#pragma once
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanImageView.h"
#include "radiant/core/render/vulkan/VulkanSampler.h"
#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

namespace Radiant {
  class Texture {
    public:
      static Texture& fromPath(std::filesystem::path path);
      static Texture& fromMemory(void* buffer);

    private:
      std::unique_ptr<VulkanImage> image;
      std::unique_ptr<VulkanImageView> imageView;
      std::unique_ptr<VulkanSampler> sampler;
  };
}
