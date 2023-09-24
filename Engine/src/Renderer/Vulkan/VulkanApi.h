#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanRenderPass.h"
#include "Renderer/Renderer.h"

namespace CeltEngine
{
    class VulkanApi
    {
    public:
        void Init();
        void Shutdown();

        VulkanInstance GetInstance() const { return m_Instance; }
        VulkanDevice GetDevice() const { return m_Device; }
    private:
        VulkanInstance m_Instance;
        VulkanDevice m_Device;
        vk::SurfaceKHR m_Surface;
        VulkanRenderPass m_MainRenderPass;
    };
}
