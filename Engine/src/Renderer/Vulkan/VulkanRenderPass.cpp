#include "cepch.h"
#include "VulkanRenderPass.h"

#include "VulkanApi.h"
#include "Core/Logger.h"

namespace CeltEngine
{
    void VulkanRenderPass::Init(VulkanDevice* device, glm::vec4 canvas, glm::vec4 clearValue,
        float depthClearValue, uint32_t stencilClearValue)
    {
        m_Canvas = canvas;
        m_ClearValue = clearValue;
        m_DepthClearValue = depthClearValue;
        m_StencilClearValue = stencilClearValue;
        m_Device = device;

        // Main SubPass
        vk::SubpassDescription subPass = {};
        subPass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

        // Attachments TODO: Make Attachments configurable
        std::vector<vk::AttachmentDescription> attachments;

        // Color Attachment
        vk::AttachmentDescription colorAttachment = {};
        colorAttachment.format = vk::Format::eB8G8R8A8Unorm; // TODO: Get image format
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eLoad;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
        attachments.push_back(colorAttachment);

        vk::AttachmentReference colorAttachmentReference = {};
        colorAttachmentReference.attachment = 0;
        colorAttachmentReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

        subPass.colorAttachmentCount = 1;
        subPass.pColorAttachments = &colorAttachmentReference;

        // Depth Attachment
        vk::AttachmentDescription depthAttachment = {};
        depthAttachment.format = vk::Format::eD32Sfloat; // TODO: Get image format
        depthAttachment.samples = vk::SampleCountFlagBits::e1;
        depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
        depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
        attachments.push_back(depthAttachment);

        vk::AttachmentReference depthAttachmentReference = {};
        depthAttachmentReference.attachment = 1;
        depthAttachmentReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        subPass.pDepthStencilAttachment = &depthAttachmentReference;

        // TODO: other attachment types (input, resolve, preserve)
        subPass.inputAttachmentCount = 0;
        subPass.pInputAttachments = nullptr;
        subPass.pResolveAttachments = nullptr;
        subPass.preserveAttachmentCount = 0;
        subPass.pPreserveAttachments = nullptr;

        // SubPass Dependencies
        std::vector<vk::SubpassDependency> subPassDependencies;

        vk::SubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcAccessMask = vk::AccessFlagBits::eNone;
        dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        subPassDependencies.push_back(dependency);

        // RenderPass
        vk::RenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subPass;
        renderPassInfo.dependencyCount = static_cast<uint32_t>(subPassDependencies.size());
        renderPassInfo.pDependencies = subPassDependencies.data();

        try {
            m_RenderPass = m_Device->GetDevice().createRenderPass(renderPassInfo);
        } catch (const vk::SystemError& err) {
            CE_ERROR("Failed to create RenderPass: %s", err.what());
            CE_BREAK;
        }
        CE_TRACE("Created PrenderPass: succesful");
    }

    void VulkanRenderPass::Shutdown()
    {
        m_Device->GetDevice().destroyRenderPass(m_RenderPass);
    }

    void VulkanRenderPass::Begin()
    {
        // TODO: Implement RenderPass::Begin()
    }

    void VulkanRenderPass::End()
    {
        // TODO: Implement RenderPass::End()
    }
}
