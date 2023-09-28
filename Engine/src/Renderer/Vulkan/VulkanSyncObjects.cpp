#include "cepch.h"
#include "VulkanSyncObjects.h"

#include "VulkanDevice.h"
#include "Core/Logger.h"

namespace CeltEngine
{
    VulkanFence::VulkanFence(VulkanDevice* device, bool isSignaled)
    {
        m_Device = device;
        
        vk::FenceCreateInfo createInfo;
        createInfo.flags = isSignaled ? vk::FenceCreateFlagBits::eSignaled : vk::FenceCreateFlags();

        try {
            m_Fence = device->GetDevice().createFence(createInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create vulkan fence: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created vulkan fence: succesful");
    }

    VulkanFence::~VulkanFence()
    {
        m_Device->GetDevice().destroyFence(m_Fence);
    }

    void VulkanFence::Reset() const
    {
        const vk::Result result = m_Device->GetDevice().resetFences(1, &m_Fence);
        if (result != vk::Result::eSuccess)
            CE_ERROR("Failed to reset vulkan fence: %s", vk::to_string(result).c_str());
    }

    void VulkanFence::Wait() const
    {
        const vk::Result result = m_Device->GetDevice().waitForFences(1, &m_Fence, VK_TRUE, UINT64_MAX);
        if (result != vk::Result::eSuccess)
            CE_ERROR("Failed to wait for vulkan fence: %s", vk::to_string(result).c_str());
    }

    void VulkanFence::WaitAndReset() const
    {
        Wait();
        Reset();
    }

    VulkanSemaphore::VulkanSemaphore(VulkanDevice* device)
    {
        m_Device = device;

        vk::SemaphoreCreateInfo createInfo;
        createInfo.flags = vk::SemaphoreCreateFlags();
        
        try {
            m_Semaphore = device->GetDevice().createSemaphore(createInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create vulkan semaphore: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created vulkan semaphore: succesful");
    }

    VulkanSemaphore::~VulkanSemaphore()
    {
        m_Device->GetDevice().destroySemaphore(m_Semaphore);
    }
}
