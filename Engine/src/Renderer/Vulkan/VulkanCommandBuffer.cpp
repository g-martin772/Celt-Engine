#include "cepch.h"
#include "VulkanCommandBuffer.h"

#include "Core/Logger.h"
#include  "VulkanDevice.h"

namespace CeltEngine
{
    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        if (!m_IsSingleUse)
            Free();
    }

    void VulkanCommandBuffer::Begin() const
    {
        vk::CommandBufferBeginInfo beginInfo;
        if(m_IsSingleUse)
            beginInfo.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        if(m_RenderPassContinue)
            beginInfo.flags |= vk::CommandBufferUsageFlagBits::eRenderPassContinue;
        if(m_SimultaneousUse)
            beginInfo.flags |= vk::CommandBufferUsageFlagBits::eSimultaneousUse;
        try {
            m_CommandBuffer.begin(beginInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to begin vulkan command buffer: %s", err.what());
            CE_BREAK;
        }
    }

    void VulkanCommandBuffer::End() const
    {
        try {
            m_CommandBuffer.end();
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to end vulkan command buffer: %s", err.what());
            CE_BREAK;
        }
    }

    void VulkanCommandBuffer::Submit(vk::Queue target) // TODO: Add sync objects
    {
        vk::SubmitInfo submitInfo;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffer;
        
        try {
            vk::Result result = target.submit(1, &submitInfo, nullptr);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to submit vulkan command buffer: %s", err.what());
            CE_BREAK;
        }
        
        if (m_IsSingleUse)
        {
            target.waitIdle();
            Free();
        }
    }

    void VulkanCommandBuffer::Free()
    {
        if(m_Allocated) {
            m_Device->GetDevice().freeCommandBuffers(m_CommandPool, 1, &m_CommandBuffer);
            m_Allocated = false;
        }
    }

    void VulkanCommandPool::Init(VulkanDevice* device, const uint32_t queueFamilyIndex, const vk::CommandPoolCreateFlags flags)
    {
        m_Device = device;
        
        vk::CommandPoolCreateInfo createInfo;
        createInfo.flags = flags;
        createInfo.queueFamilyIndex = queueFamilyIndex;

        try {
            m_CommandPool = m_Device->GetDevice().createCommandPool(createInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create vulkan command pool: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created vulkan command pool: succesful");
    }

    void VulkanCommandPool::Destroy()
    {
        m_Device->GetDevice().destroyCommandPool(m_CommandPool);
    }

    Shared<VulkanCommandBuffer> VulkanCommandPool::AllocateCommandBuffer(bool renderPassContinue, bool simultaneousUse) const
    {
        Shared<VulkanCommandBuffer> commandBuffer = std::make_shared<VulkanCommandBuffer>();
        commandBuffer->m_Device = m_Device;
        commandBuffer->m_CommandPool = m_CommandPool;
        commandBuffer->m_SimultaneousUse = simultaneousUse;
        commandBuffer->m_RenderPassContinue = renderPassContinue;
        commandBuffer->m_Allocated = true;

        vk::CommandBufferAllocateInfo allocateInfo;
        allocateInfo.level = vk::CommandBufferLevel::ePrimary;
        allocateInfo.commandBufferCount = 1;
        allocateInfo.commandPool = m_CommandPool;

        try {
            commandBuffer->m_CommandBuffer = m_Device->GetDevice().allocateCommandBuffers(allocateInfo)[0];
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to allocate vulkan command buffer: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Allocated vulkan command buffer: succesful");

        return commandBuffer;
    }

    Shared<VulkanCommandBuffer> VulkanCommandPool::AllocateSingleUseCommandBuffer()
    {
        Shared<VulkanCommandBuffer> buffer = AllocateCommandBuffer();
        buffer->m_IsSingleUse = true;
        return buffer;
    }
}

