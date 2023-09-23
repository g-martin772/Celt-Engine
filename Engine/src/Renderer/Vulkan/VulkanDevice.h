﻿#pragma once

#include <vulkan/vulkan.hpp>

#include "Core/Core.h"

namespace CeltEngine
{
    class VulkanInstance;
}

namespace CeltEngine
{
    struct DeviceRequirements
    {
        bool Graphics, Compute, Transfer, Sparse;
    };

    struct VulkanQueueIndices
    {
        uint32_t Graphics = -1, Compute = -1, Transfer = -1, Sparse = -1;
    };
    
    class VulkanDevice
    {
    public:
        void Init(DeviceRequirements requirements, VulkanInstance* instance);
        void Shutdown();
        
        vk::Device GetDevice() const { return m_Device; }
        vk::PhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
        VulkanQueueIndices GetQueueIndices() const { return m_QueueIndices; }
    private:
        VulkanInstance* m_Instance = nullptr;
        vk::Device m_Device;
        vk::PhysicalDevice m_PhysicalDevice;
        VulkanQueueIndices m_QueueIndices;
    };
}
