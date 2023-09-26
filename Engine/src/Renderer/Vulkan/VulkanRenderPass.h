﻿#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

namespace CeltEngine
{
    class VulkanRenderPass
    {
    public:
        void Init(::CeltEngine::VulkanDevice* device, ::CeltEngine::VulkanSwapChain* swapchain, glm::vec4 canvas, glm::vec4 clearValue =
                  {
                      0.0f, 0.0f, 0.0f, 1.0f
                  }, float depthClearValue = 1.0f, uint32_t
                  stencilClearValue = 0);
        void Destroy();

        void Begin();
        void End();

        vk::RenderPass GetRenderPass() const { return m_RenderPass; }
    private:
        vk::RenderPass m_RenderPass;
        float m_DepthClearValue = 1.0f;
        uint32_t m_StencilClearValue = 0.0f;
        glm::vec4 m_ClearValue = {};
        glm::vec4 m_Canvas = {};

        VulkanDevice* m_Device = nullptr;
    };
}
