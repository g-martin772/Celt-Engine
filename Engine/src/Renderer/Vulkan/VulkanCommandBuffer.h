#pragma once

#include <vulkan/vulkan.hpp>

namespace CeltEngine
{
    class VulkanDevice;
}

namespace CeltEngine
{
    class VulkanCommandBuffer
    {
    public:
        // Use the VulkanCommandPool to allocate a Vulkan command buffer!
        VulkanCommandBuffer() = default;
        ~VulkanCommandBuffer();

        void Begin() const;
        void End() const;
        void Submit(vk::Queue target) const;

        void Free() const;
        
        vk::CommandBuffer GetCommandBuffer() const { return m_CommandBuffer; }
    private:
        vk::CommandBuffer m_CommandBuffer;
        bool m_IsSingleUse = false, m_RenderPassContinue = false, m_SimultaneousUse = false;
        vk::CommandPool m_CommandPool;
        VulkanDevice* m_Device = nullptr;
        friend class VulkanCommandPool;
    };
    
    class VulkanCommandPool
    {
    public:
        void Init(VulkanDevice* device, uint32_t queueFamilyIndex, vk::CommandPoolCreateFlags flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
        void Destroy();

        std::shared_ptr<VulkanCommandBuffer> AllocateCommandBuffer(bool renderPassContinue = false, bool simultaneousUse = false) const;
        std::shared_ptr<VulkanCommandBuffer> AllocateSingleUseCommandBuffer();
    private:
        VulkanDevice* m_Device = nullptr;
        vk::CommandPool m_CommandPool;
    };
}
