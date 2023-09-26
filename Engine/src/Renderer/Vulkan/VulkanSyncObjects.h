#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"

namespace CeltEngine
{
    class VulkanFence
    {
    public:
        void Create(VulkanDevice* device, bool isSignaled = true);
        void Destroy() const;

        void Reset() const;
        void Wait() const;
        void WaitAndReset() const;

        vk::Fence GetFence() const { return m_Fence; }
    private:
        vk::Fence m_Fence;
        VulkanDevice* m_Device = nullptr;
    };

    class VulkanSemaphore
    {
    public:
        void Create(VulkanDevice* device);
        void Destroy() const;

        vk::Semaphore GetSemaphore() const { return m_Semaphore; }
    private:
        vk::Semaphore m_Semaphore;
        VulkanDevice* m_Device = nullptr;
    };
}
