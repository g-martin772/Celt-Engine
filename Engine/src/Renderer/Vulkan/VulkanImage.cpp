#include "cepch.h"
#include "VulkanImage.h"

#include "VulkanDevice.h"
#include "VulkanMemory.h"
#include "Core/Logger.h"

namespace CeltEngine
{
    void VulkanImage::Create(const VulkanImageSpec& spec, VulkanDevice* device)
    {
        m_Size = spec.Size;
        m_Device = device;

        vk::ImageCreateInfo imageInfo = {};
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = static_cast<uint32_t>(spec.Size.x);
        imageInfo.extent.height = static_cast<uint32_t>(spec.Size.y);
        imageInfo.extent.depth = spec.Depth;
        imageInfo.mipLevels = spec.MipLevels;
        imageInfo.arrayLayers = spec.ArrayLayers;
        imageInfo.format = spec.Format;
        imageInfo.tiling = spec.Tiling;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = spec.Usage;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.sharingMode = spec.ExclusiveSharing ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent;

        try {
            m_Image = m_Device->GetDevice().createImage(imageInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create Vulkan Image: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created Vulkan Image: succesful");

        // Get memory requirements
        vk::MemoryRequirements requirements = m_Device->GetDevice().getImageMemoryRequirements(m_Image);
        const uint32_t  memoryIndex = VulkanMemory::FindMemoryIndex(m_Device, requirements.memoryTypeBits, spec.MemoryProperties);

        // Allocate Image
        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = requirements.size;
        allocInfo.memoryTypeIndex = memoryIndex;
        
        try {
            m_Memory = m_Device->GetDevice().allocateMemory(allocInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to allocate vulkan image memory: %s", err.what());
            CE_BREAK;
        }

        try {
            m_Device->GetDevice().bindImageMemory(m_Image, m_Memory, 0);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to bind vulkan image memory: %s", err.what());
            CE_BREAK;
        }

        if(spec.CreateView) {
            VulkanImageViewSpec viewSpec;
            viewSpec.Type = spec.ViewType;
            viewSpec.Format = spec.Format;
            viewSpec.AspectFlags = spec.ViewAspectFlags;
            viewSpec.ArrayLayers = spec.ArrayLayers;
            CreateImageView(viewSpec);
        }
    }

    void VulkanImage::Destroy()
    {
        for(const auto& view : m_ImageViews)
            m_Device->GetDevice().destroyImageView(view);
        m_Device->GetDevice().freeMemory(m_Memory);
        m_Device->GetDevice().destroyImage(m_Image);
    }

    uint32_t VulkanImage::CreateImageView(const VulkanImageViewSpec& spec)
    {
        vk::ImageViewCreateInfo createInfo = {};
        createInfo.image = m_Image;
        createInfo.viewType = spec.Type;
        createInfo.format = spec.Format;
        createInfo.subresourceRange.aspectMask = spec.AspectFlags;
        createInfo.subresourceRange.baseMipLevel = spec.BaseMipLevel;
        createInfo.subresourceRange.levelCount = spec.ArrayLayers;
        createInfo.subresourceRange.baseArrayLayer = spec.BaseArrayLayer;

        try {
            m_ImageViews.push_back(m_Device->GetDevice().createImageView(createInfo));
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create vulkan image view: %s", err.what());
            CE_BREAK;
        }

        return m_ImageViews.size() - 1;
    }
}
