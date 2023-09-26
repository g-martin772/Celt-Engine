#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanFrameBuffer.h"
#include "VulkanInstance.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapChain.h"
#include "Renderer/Renderer.h"

namespace CeltEngine
{
    class VulkanApi
    {
    public:
        void Init();
        void Shutdown();

        void OnResize(glm::vec2 newSize);

        VulkanInstance GetInstance() const { return m_Instance; }
        VulkanDevice GetDevice() const { return m_Device; }
    private:
        VulkanInstance m_Instance;
        VulkanDevice m_Device;
        vk::SurfaceKHR m_Surface;
        VulkanRenderPass m_MainRenderPass;
        VulkanSwapChain m_SwapChain;
        VulkanCommandPool m_GraphicsCommandPool;
        Shared<VulkanCommandBuffer> m_MainCommandBuffer;
        VulkanFrameBuffer m_MainFrameBuffer;
        friend class VulkanRenderCommand;
    };
}
