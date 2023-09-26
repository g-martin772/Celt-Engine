﻿#include "cepch.h"
#include "VulkanFrameBuffer.h"

#include "VulkanRenderPass.h"
#include "VulkanSwapChain.h"
#include "Core/Application.h"
#include "Core/Logger.h"

namespace CeltEngine
{
    // TODO: Support multiple attachments
    void VulkanFrameBuffer::Create(VulkanDevice* device, glm::vec2 size, std::vector<vk::ImageView> imageViews, VulkanRenderPass* renderPass)
    {
        m_RenderPass = renderPass;
        m_Device = device;

        for (auto& imageView : imageViews)
        {
            vk::FramebufferCreateInfo createInfo;
            createInfo.renderPass = m_RenderPass->GetRenderPass();
            createInfo.attachmentCount = 1;
            createInfo.pAttachments = &imageView;
            createInfo.width = size.x;
            createInfo.height = size.y;
            createInfo.layers = 1;

            try {
                m_FrameBuffers.push_back(m_Device->GetDevice().createFramebuffer(createInfo));
            } catch (const vk::SystemError& err) {
                CE_ERROR("Failed to create vulkan frame buffer: %s", err.what());
                CE_BREAK;
            }
            CE_TRACE("Created vulkan frame buffer: succesful");
        }
    }

    void VulkanFrameBuffer::CreateFromSwapchain(VulkanDevice* device, VulkanSwapChain* swapChain, VulkanRenderPass* renderPass)
    {
        m_RenderPass = renderPass;
        m_Device = device;

        const auto imageViews = swapChain->GetImageViews();

        for (auto& imageView : imageViews)
        {
            vk::ImageView attachments[2] = { imageView, swapChain->GetDepthImageView() };
            
            vk::FramebufferCreateInfo createInfo;
            createInfo.renderPass = m_RenderPass->GetRenderPass();
            createInfo.attachmentCount = 2;
            createInfo.pAttachments = attachments;
            createInfo.width = swapChain->GetExtent().width;
            createInfo.height = swapChain->GetExtent().height;
            createInfo.layers = 1;

            try {
                m_FrameBuffers.push_back(m_Device->GetDevice().createFramebuffer(createInfo));
            } catch (const vk::SystemError& err) {
                CE_ERROR("Failed to create vulkan frame buffer: %s", err.what());
                CE_BREAK;
            }
            CE_TRACE("Created vulkan frame buffer: succesful");
        }
    }

    void VulkanFrameBuffer::Destroy()
    {
        for (vk::Framebuffer framebuffer : m_FrameBuffers)
            m_Device->GetDevice().destroyFramebuffer(framebuffer);
    }
}
